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
#include <algorithm>

auto
find_async_task
(
    std::vector<async_task_info_type> & async_tasks,
    int id
)
{
    return
        std::find_if
        (   
            async_tasks.begin(),
            async_tasks.end(),
            [&id](auto const & task)
            {
                return task.id == id;
            }
        );
}

state_type reducer(state_type state, action_type action)
{
    if (std::holds_alternative<add_async_task_info_action_type>(action))
    {
        auto a = std::get<add_async_task_info_action_type>(action);
        if (find_async_task(state.async_tasks, a.id) == state.async_tasks.end())
            state.async_tasks.push_back(async_task_info_type{a.id, 0});
    }
    else if (std::holds_alternative<remove_async_task_info_action_type>(action))
    {
        auto a = std::get<remove_async_task_info_action_type>(action);
        auto it = find_async_task(state.async_tasks, a.id);
        if (it != state.async_tasks.end())
            state.async_tasks.erase(it);
    }
    else if (std::holds_alternative<set_async_task_info_progress_action_type>(action))
    {
        auto a = std::get<set_async_task_info_progress_action_type>(action);
        auto it = find_async_task(state.async_tasks, a.id);
        if (it != state.async_tasks.end())
            (*it).progress = a.progress;
    }
            
    return state;
}
