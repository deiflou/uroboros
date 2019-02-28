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

#ifndef EXAMPLE_TYPES_HPP
#define EXAMPLE_TYPES_HPP

#include <uroboros/store.hpp>
#include <uroboros/middleware_enhancer.hpp>
#include <variant>
#include <vector>

struct async_task_info_type
{
    int id;
    int progress;
};

struct state_type
{
    std::vector<async_task_info_type> async_tasks{};
};

struct start_async_task_action_type
{
    int duration{1};
};

struct cancel_async_task_action_type
{
    int id;
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

using action_type =
    std::variant
    <
        start_async_task_action_type,
        cancel_async_task_action_type,
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

using dispatch_transformer_type =
    typename store_type::dispatch_transformer_type;
using dispatch_function_type =
    typename store_type::dispatch_function_type;
using middleware_api_type =
    store_type::middleware_api;

#endif // EXAMPLE_TYPES_HPP
