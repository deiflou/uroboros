//
// MIT License
//
// Copyright (c) 2019 Deif Lou
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include "types.hpp"
#include <stlab/concurrency/future.hpp>
#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/main_executor.hpp>
#include <chrono>
#include <unordered_map>
#include <mutex>
#include <iostream>

enum class async_task_status_type
{
    waiting_to_start,
    started,
    canceled_before_starting,
    canceled
};

static int current_task_id{0};
static std::unordered_map<int, async_task_status_type> async_task_status;
static std::mutex async_task_status_mutex;

void
task(middleware_api_type api, int task_id, int duration)
{
    {
        std::lock_guard lock_guard(async_task_status_mutex);
        if
        (
            async_task_status.at(task_id) ==
            async_task_status_type::canceled_before_starting
        )
            return;

        async_task_status.at(task_id) = async_task_status_type::started;
    }

    std::chrono::milliseconds sleep_duration =
        std::chrono::milliseconds{duration * 1000 / 20};

    for (int i = 0; i < 20; ++i)
    {
        std::this_thread::sleep_for(sleep_duration);

        {
            std::lock_guard lock_guard(async_task_status_mutex);
            if (async_task_status.at(task_id) == async_task_status_type::canceled)
                break;
        }

        stlab::async
        (
            stlab::main_executor,
            [api, task_id, progress = i * 5]()
            {
                api.dispatch
                    (
                        set_async_task_info_progress_action_type
                        {
                            task_id,
                            progress
                        }
                    );
            }
        ).detach();
    }
}

void
handle_finishing_async_task(middleware_api_type api, int task_id)
{
    async_task_status_type status;

    {
        std::lock_guard lock_guard(async_task_status_mutex);
        status = async_task_status.at(task_id);
        if (status == async_task_status_type::canceled_before_starting)
            return;
        async_task_status.erase(task_id);
    }

    if (status != async_task_status_type::canceled)
        api.dispatch(set_async_task_info_progress_action_type{task_id, 100});
    api.dispatch(remove_async_task_info_action_type{task_id});
}

void
handle_start_async_task_action(middleware_api_type api, int task_id, int duration)
{
    {
        std::lock_guard lock_guard(async_task_status_mutex);
        async_task_status[task_id] = async_task_status_type::waiting_to_start;
    }
    
    api.dispatch(add_async_task_info_action_type{task_id});

    stlab::async(stlab::default_executor, task, api, task_id, duration)
        .then
        (
            stlab::main_executor,
            [api, task_id]()
            {
                handle_finishing_async_task(api, task_id);
            }
        )
        .detach();
}

void
handle_cancel_async_task_action(middleware_api_type api, int task_id)
{
    std::lock_guard lock_guard(async_task_status_mutex);

    auto & status = async_task_status.at(task_id);
    if (status == async_task_status_type::waiting_to_start)
    {
        status = async_task_status_type::canceled_before_starting;
        api.dispatch(remove_async_task_info_action_type{task_id});
    }
    else
        status = async_task_status_type::canceled;
}

dispatch_transformer_type
async_middleware(middleware_api_type api)
{
    return
        [=](dispatch_function_type next) -> dispatch_function_type
        {
            return
                [=](action_type action) -> action_type
                {
                    if
                    (
                        !std::holds_alternative<start_async_task_action_type>(action) &&
                        !std::holds_alternative<cancel_async_task_action_type>(action)
                    )
                        return next(action);

                    if (std::holds_alternative<start_async_task_action_type>(action))
                        handle_start_async_task_action
                        (
                            api,
                            current_task_id++,
                            std::get<start_async_task_action_type>(action).duration
                        );
                    else if (std::holds_alternative<cancel_async_task_action_type>(action))
                        handle_cancel_async_task_action
                        (
                            api,
                            std::get<cancel_async_task_action_type>(action).id
                        );

                    return action;
                };
        };
}
