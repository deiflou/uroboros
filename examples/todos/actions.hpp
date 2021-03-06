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

#ifndef EXAMPLE_ACTIONS_HPP
#define EXAMPLE_ACTIONS_HPP

#include "state.hpp"
#include <variant>

struct add_todo_action_type
{
    std::string text;
};

struct delete_todo_action_type
{
    int id;
};

struct toggle_todo_action_type
{
    int id;
};

struct set_visibility_action_type
{
    visibility_filter_type visibility_filter;
};

using action_type =
    std::variant
    <
        add_todo_action_type,
        delete_todo_action_type,
        toggle_todo_action_type,
        set_visibility_action_type
    >;

#endif // EXAMPLE_ACTIONS_HPP
