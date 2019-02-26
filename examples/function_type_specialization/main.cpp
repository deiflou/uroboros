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

#include <functional>

#ifdef NAIOS_FUNCTION2_FOUND
#include <function2/function2.hpp>
#endif

#ifdef PMED_FIXED_SIZE_FUNCTION_FOUND
#include <fixed_size_function/fixed_size_function.hpp>
#endif

#ifdef SKARUPKE_STD_FUNCTION_FOUND
#include <std_function/function.h>
#endif

using state_type = int;
using action_type = std::string;

using store1_type = uroboros::store
                    <
                        state_type,
                        action_type,
                        std::function
                    >;

#ifdef NAIOS_FUNCTION2_FOUND
using store2_type = uroboros::store
                    <
                        state_type,
                        action_type,
                        fu2::template function
                    >;
#endif

#ifdef PMED_FIXED_SIZE_FUNCTION_FOUND
template <typename Signature_Type>
using fixed_size_function_type = fixed_size_function<Signature_Type, 256>;
using store3_type = uroboros::store
                    <
                        state_type,
                        action_type,
                        fixed_size_function_type
                    >;
#endif

#ifdef SKARUPKE_STD_FUNCTION_FOUND
using store4_type = uroboros::store
                    <
                        state_type,
                        action_type,
                        func::function
                    >;
#endif

state_type
reducer(state_type state, action_type action)
{
    // Log the action. Note that this is done for illustration purposes only.
    // Reducers should be pure functions and shouldn't have any side effects
    std::cout << "Executing action "
              << action
              << std::endl;

    if (action == "+")
        return state + 1;
    else if (action == "-")
        return state - 1;

    return state;
}

template <typename Store_Type>
void
subscriber(Store_Type & store)
{
    std::cout << "Subscriber notified. State = "
              << store.state()
              << std::endl;
}

template <typename Store_Type>
void
dispatch(Store_Type & store)
{
    // Dispatch dummy "init" action, just to make
    // the subscriber display the message
    store.dispatch("init");

    // Dipatch single actions
    store.dispatch("+");
    store.dispatch("+");
    store.dispatch("+");
    store.dispatch("-");
}

int
main()
{
    // std::function
    std::cout << "STD FUNCTION" << std::endl;
    store1_type store1(reducer, 0);
    store1.subscribe([&store1](){ subscriber(store1); });
    dispatch(store1);
    
#ifdef NAIOS_FUNCTION2_FOUND
    std::cout << "\nNAIOS' FUNCTION2" << std::endl;
    store2_type store2(reducer, 0);
    store2.subscribe([&store2](){ subscriber(store2); });
    dispatch(store2);
#endif

#ifdef PMED_FIXED_SIZE_FUNCTION_FOUND
    std::cout << "\nPMED's FIXED_SIZE_FUNCTION" << std::endl;
    store3_type store3(reducer, 0);
    store3.subscribe([&store3](){ subscriber(store3); });
    dispatch(store3);
#endif

#ifdef SKARUPKE_STD_FUNCTION_FOUND
    std::cout << "\nSKARUPKE's STD_FUNCTION" << std::endl;
    store4_type store4(reducer, 0);
    store4.subscribe([&store4](){ subscriber(store4); });
    dispatch(store4);
#endif

    return 0;
}
