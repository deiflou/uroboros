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
#include <iostream>
#include <string>

using state_type = int;
using action_type = std::string;
using store_type = uroboros::store<state_type, action_type>;

state_type
reducer(state_type state, action_type action)
{
    if (action == "+")
        return state + 1;
    else if (action == "-")
        return state - 1;

    return state;
}

void
view(store_type const & store)
{
    std::cout << "State = " << store.state() << std::endl;
}

void
info()
{
    std::cout << "List of actions:\n"
              << "\t* q : quit application\n"
              << "\t* + : increment counter\n"
              << "\t* - : decrement counter\n"
              << std::endl;
}

int
main()
{
    store_type store(reducer, 0);

    store.subscribe(
        [&store]()
        {
            view(store);
        }
    );

    info();

    view(store);

    action_type action;

    while (true)
    {
        std::cin >> action;
        if (action == "q")
            break;

        if (action == "+" || action == "-")
            store.dispatch(action);
        else
        {
             std::cout << "Unknown action\n\n";
             info();
        }
    }

    return 0;
}
