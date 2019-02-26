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

#include <uroboros/store.hpp>
#include <uroboros/middleware_enhancer.hpp>
#include "../common/task_manager.hpp"
#include "../common/executor.hpp"
#include <stlab/concurrency/future.hpp>
#include <stlab/concurrency/default_executor.hpp>
#include <thread>
#include <iostream>
#include <string>
#include <variant>
#include <chrono>
#include <cctype>
#include <algorithm>
#include <vector>

struct async_task_info_type
{
    int id;
    int progress;
};

struct state_type
{
    std::string text{"initial text"};
    std::vector<async_task_info_type> async_tasks{};
};

struct set_text_action_type
{
    std::string text;
};

struct set_raw_text_action_type
{
    std::string text;
    std::chrono::milliseconds duration;
};

struct add_async_task_info_action_type
{
    int id;
};

struct remove_async_task_info_action_type
{
    int id;
};

struct set_async_task_info_progress_action_type
{
    int id;
    int progress;
};

using action_type = std::variant
                    <
                        set_text_action_type,
                        set_raw_text_action_type,
                        add_async_task_info_action_type,
                        remove_async_task_info_action_type,
                        set_async_task_info_progress_action_type
                    >;

using store_type =
    uroboros::enhanced_store
    <
        uroboros::store<state_type, action_type>,
        uroboros::middleware_enhancer
    >;

using task_manager = uroboros::examples::task_manager;

using dispatch_transformer_type =
    typename store_type::dispatch_transformer_type;
using dispatch_function_type =
    typename store_type::dispatch_function_type;
using middleware_api_type =
    store_type::middleware_api;

dispatch_transformer_type
toupper_middleware(middleware_api_type api)
{
    return
        [=](dispatch_function_type next) -> dispatch_function_type
        {
            return
                [=](action_type action) -> action_type
                {
                    if (!std::holds_alternative<set_raw_text_action_type>(action))
                        return next(action);
                    
                    static int next_task_id{0};

                    api.dispatch(add_async_task_info_action_type{next_task_id++});

                    stlab::async
                    (
                        stlab::default_executor,
                        [api, task_id = next_task_id - 1]
                        (std::string str, std::chrono::milliseconds duration)
                        {
                            std::transform
                            (
                                str.begin(),
                                str.end(),
                                str.begin(),
                                [](unsigned char c)
                                {
                                    return std::toupper(c);
                                }
                            );

                            std::chrono::milliseconds sleep_duration = duration / 20;

                            for (int i = 0; i < 20; ++i)
                            {
                                std::this_thread::sleep_for(sleep_duration);
                                stlab::async
                                (
                                    uroboros::examples::executor,
                                    [api, task_id, i]()
                                    {
                                        api.dispatch
                                        (
                                            set_async_task_info_progress_action_type
                                            {
                                                task_id,
                                                i * 5
                                            }
                                        );
                                    }
                                ).detach();
                            }

                            return str;
                        },
                        std::get<set_raw_text_action_type>(action).text,
                        std::get<set_raw_text_action_type>(action).duration
                    ).then
                    (
                        uroboros::examples::executor,
                        [api, task_id = next_task_id - 1]
                        (std::string str)
                        {
                            api.dispatch(set_async_task_info_progress_action_type{task_id, 100});
                            api.dispatch(remove_async_task_info_action_type{task_id});
                            api.dispatch(set_text_action_type{str});
                        }
                    ).detach();

                    return action;
                };
        };
}

state_type reducer(state_type state, action_type action)
{
    if (std::holds_alternative<set_text_action_type>(action))
        state.text = std::get<set_text_action_type>(action).text;

    else if (std::holds_alternative<add_async_task_info_action_type>(action))
    {
        auto a = std::get<add_async_task_info_action_type>(action);
        auto it =
            std::find_if
            (   
                state.async_tasks.begin(),
                state.async_tasks.end(),
                [&a](auto const & task)
                {
                    return task.id == a.id;
                }
            );

        if (it == state.async_tasks.end())
            state.async_tasks.push_back(async_task_info_type{a.id, 0});
    }

    else if (std::holds_alternative<remove_async_task_info_action_type>(action))
    {
        auto a = std::get<remove_async_task_info_action_type>(action);
        auto it =
            std::find_if
            (   
                state.async_tasks.begin(),
                state.async_tasks.end(),
                [&a](auto const & task)
                {
                    return task.id == a.id;
                }
            );

        if (it != state.async_tasks.end())
            state.async_tasks.erase(it);
    }

    else if (std::holds_alternative<set_async_task_info_progress_action_type>(action))
    {
        auto a = std::get<set_async_task_info_progress_action_type>(action);
        auto it =
            std::find_if
            (   
                state.async_tasks.begin(),
                state.async_tasks.end(),
                [&a](auto const & task)
                {
                    return task.id == a.id;
                }
            );

        if (it != state.async_tasks.end())
            (*it).progress = a.progress;
    }
            
    return state;
}

void
view(store_type const & store)
{
    std::cout << "Current text: " << store.state().text << std::endl;
    std::cout << "Pending tasks: " << store.state().async_tasks.size() << std::endl;
    for (async_task_info_type const & task_info : store.state().async_tasks)
    {
        int const total_progress_width = 50;
        int const progress_width = task_info.progress * total_progress_width / 100;
        std::cout
            << "    * Task "
            << task_info.id
            << " progress: ["
            << std::string(progress_width, '*')
            << std::string(total_progress_width - progress_width, ' ')
            << "]"
            << std::endl;
    }
}

int
main()
{
    store_type store(reducer, {});
    store.apply_middleware(toupper_middleware);
    store.subscribe
    (
        [&store]()
        {
            view(store);
        }
    );

    using namespace std::literals::chrono_literals;
    
    auto n = std::chrono::steady_clock::now();
    store.dispatch(set_raw_text_action_type{"text number one", 4000ms});
    store.dispatch(set_raw_text_action_type{"text number two", 2000ms});
    store.dispatch(set_raw_text_action_type{"text number three", 6000ms});

    while (store.state().async_tasks.size() > 0)
    {
        task_manager::instance().execute_all_pending_tasks();
        std::this_thread::sleep_for(50ms);
    }
    auto d = std::chrono::steady_clock::now() - n;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(d).count() << std::endl;
}
