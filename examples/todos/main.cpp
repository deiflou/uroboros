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
#include <string>
#include <iostream>
#include <optional>
#include <sstream>
#include <vector>
#include <cctype>
#include <algorithm>

extern
state_type
reducer(state_type, action_type);

bool
is_number(std::string const & str)
{
    return
        std::all_of
        (
            str.begin(),
            str.end(),
            [](char c) -> bool
            {
                return std::isdigit(c);
            }
        );
}
std::vector<std::string>
input_to_tokens(std::string const & input)
{
    std::stringstream stream(input);
    std::vector<std::string> tokens;

    while (!stream.eof())
    {
        std::string token;
        stream >> token;
        tokens.push_back(token);
    }

    return tokens;
}

std::optional<action_type>
input_to_action(std::string const & input)
{
    std::vector<std::string> tokens = input_to_tokens(input);
    if (tokens.size() < 2)
        return std::nullopt;

    if (tokens[0] == "add")
    {
        std::string text;
        for (int i = 1; i < tokens.size(); ++i)
            text = text + tokens[i] + " ";

        return add_todo_action_type{text};
    }

    if (tokens.size() > 2)
        return std::nullopt;

    if (tokens[0] == "del")
    {
        if (is_number(tokens[1]))
            return delete_todo_action_type{std::stoi(tokens[1])};
    }
    else if (tokens[0] == "toggle")
    {
        if (is_number(tokens[1]))
            return toggle_todo_action_type{std::stoi(tokens[1])};
    }
    else if (tokens[0] == "filter")
    {
        if (tokens[1] == "all")
            return set_visibility_action_type{visibility_filter_type::show_all};
        else if (tokens[1] == "completed")
            return set_visibility_action_type{visibility_filter_type::show_completed};
        else if (tokens[1] == "active")
            return set_visibility_action_type{visibility_filter_type::show_active};
    }

    return std::nullopt;
}

void
show_info()
{
    std::cout
        << "Available actions:\n"
        << "    * quit: exits the app\n"
        << "    * info: show this info\n"
        << "    * list: show the list of todos\n"
        << "    * add <text>: adds a todo with the given text\n"
        << "    * del <index>: deletes the todo with the given index\n"
        << "    * toggle <index>: toggles the status of the todo with the given index\n"
        << "    * filter <all|completed|active>: show only the filtered todos in the list\n"
        << std::endl;
}

void
list_all_todos(std::vector<todo_type> const & todos)
{
    std::cout << "Showing all the todos\n";
    if (todos.size() == 0)
    {
        std::cout << "    - There are no todos\n";
        return;
    }

    for (int i = 0; i < todos.size(); ++i)
        std::cout
            << i << ". [" << (todos[i].is_completed ? "v" : " ") << "] "
            << todos[i].text << "\n";

    std::cout << std::endl;
}

void
list_completed_todos(std::vector<todo_type> const & todos)
{
    std::cout << "Showing completed todos\n";
    bool there_are_todos{false};

    for (int i = 0; i < todos.size(); ++i)
        if (todos[i].is_completed)
        {
            there_are_todos = true;
            std::cout << i << ". " << todos[i].text << "\n";
        }

    if (!there_are_todos)
        std::cout << "    - There are no completed todos\n";

    std::cout << std::endl;
}

void
list_active_todos(std::vector<todo_type> const & todos)
{
    std::cout << "Showing active todos\n";
    bool there_are_todos{false};

    for (int i = 0; i < todos.size(); ++i)
        if (!todos[i].is_completed)
        {
            there_are_todos = true;
            std::cout << i << ". " << todos[i].text << "\n";
        }

    if (!there_are_todos)
        std::cout << "    - There are no active todos\n";

    std::cout << std::endl;
}

void
list_todos(store_type const & store)
{
    auto filter = store.state().visibility_filter;
    if (filter == visibility_filter_type::show_all)
        list_all_todos(store.state().todos);
    else if (filter == visibility_filter_type::show_completed)
        list_completed_todos(store.state().todos);
    else
        list_active_todos(store.state().todos);
}

int
main()
{
    store_type store(reducer, {});

    show_info();

    while (true)
    {
        std::string input;
        std::getline(std::cin, input);

        if (input == "quit")
            break;

        if (input == "info")
        {
            show_info();
            continue;
        }

        if (input == "list")
        {
            list_todos(store);
            continue;
        }

        auto action = input_to_action(input);
        if (action)
            store.dispatch(*action);
        else
            std::cout << "Invalid command" << std::endl;
    }

    return 0;
}
