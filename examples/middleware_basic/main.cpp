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
#include <iostream>
#include <string>

using state_type = int;
using action_type = std::string;
// Get the type of the store enhanced with the "middleware_enhancer"
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

dispatch_transformer_type
middleware_1(middleware_api_type api)
{
    std::cout << "* Creating middleware 1" << std::endl;
    return
        [=](dispatch_function_type next) -> dispatch_function_type
        {
            return
                [=](action_type action) -> action_type
                {
                    std::cout
                        << "Entering middleware 1: action = "
                        << action
                        << ", state = "
                        << api.state()
                        << std::endl;

                    action_type a = next(action);

                    std::cout
                        << "Leaving middleware 1: action = "
                        << action
                        << ", state = "
                        << api.state()
                        << std::endl;

                    return a;
                };
        };
}

dispatch_transformer_type
middleware_2(middleware_api_type api)
{
    std::cout << "* Creating middleware 2" << std::endl;
    return
        [=](dispatch_function_type next) -> dispatch_function_type
        {
            return
                [=](action_type action) -> action_type
                {
                    std::cout
                        << "Entering middleware 2: action = "
                        << action
                        << ", state = "
                        << api.state()
                        << std::endl;

                    action_type a = next(action);

                    std::cout
                        << "Leaving middleware 2: action = "
                        << action
                        << ", state = "
                        << api.state()
                        << std::endl;

                    return a;
                };
        };
}

int
main()
{
    store_type store
    (
        [](auto state, auto action)
        {
            return state + 1;
        },
        0
    );

    store.subscribe(
        []()
        {
            std::cout << "State change notified." << std::endl;
        }
    );

    store.apply_middleware(middleware_1, middleware_2);

    store.dispatch("hello");
    store.dispatch("world!");

    return 0;
}
