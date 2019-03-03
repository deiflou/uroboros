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

visibility_filter_type
visibility_filter_reducer
(
    visibility_filter_type visibility_filter,
    action_type action
)
{
    if (std::holds_alternative<set_visibility_action_type>(action))
        return std::get<set_visibility_action_type>(action).visibility_filter;
    return visibility_filter;
}

std::vector<todo_type>
todos_reducer
(
    std::vector<todo_type> todos,
    action_type action
)
{
    if (std::holds_alternative<add_todo_action_type>(action))
    {
        todos.push_back
        (
            {
                std::get<add_todo_action_type>(action).text,
                false
            }
        );
    }
    else if (std::holds_alternative<delete_todo_action_type>(action))
    {
        int id = std::get<delete_todo_action_type>(action).id;
        if (id >= 0 && id < todos.size())
            todos.erase(todos.begin() + id);
    }
    else if (std::holds_alternative<toggle_todo_action_type>(action))
    {
        int id = std::get<toggle_todo_action_type>(action).id;
        if (id >= 0 && id < todos.size())
            todos[id].is_completed = !todos[id].is_completed;
    }

    return todos;
}

state_type
reducer(state_type state, action_type action)
{
    return
    {
        visibility_filter_reducer(state.visibility_filter, action),
        todos_reducer(std::move(state.todos), action)
    };
}
