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

#include "../common/batched_actions_enhancer.hpp"
#include <uroboros/store.hpp>
#include <iostream>
#include <string>

using state_type = int;
using action_type = std::string;
// Get the type of the store enhanced with the "batched_actions_enhancer"
using store_type =
    uroboros::enhanced_store
    <
        uroboros::store<state_type, action_type>,
        uroboros::examples::batched_actions_enhancer
    >;

int
main()
{
    store_type store
    (
        [](auto state, auto action)
        {
            // Log the action. Note that this is done for illustration
            // purposes only. Reducers should be pure functions and
            // shouldn't have any side effects
            std::cout << "Executing action \""
                      << action
                      << "\""
                      << std::endl;

            if (action == "+")
                return state + 1;
            else if (action == "-")
                return state - 1;

            return state;
        },
        0
    );

    store.subscribe
    (
        [&store]()
        {
            std::cout << "State = " << store.state() << std::endl;
        }
    );
    
    // Dispatch dummy "init" action, just to make the
    // subscriber display the message
    store.dispatch("init");

    // Dispatch single actions
    store.dispatch("+");
    store.dispatch("+");
    store.dispatch("+");
    store.dispatch("-");

    // Dispatch batched actions
    store.dispatch({"+", "+", "+", "-"});

    // Dispatch single actions
    store.dispatch("+");
    store.dispatch("+");

    return 0;
}
