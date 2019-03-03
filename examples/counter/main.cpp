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

// Declare some handy type aliases.
// The state type of this app is a simple "int" that holds the value
// of the counter.
// The action type is a "std::string". In this app we don't need any
// complex type with extra information for the actions.
// "store_type" is just a shorthand alias for a store which state
// and actions are of type "state_type" and "action_type" respectively.
using state_type = int;
using action_type = std::string;
using store_type = uroboros::store<state_type, action_type>;

// This function sole purpose is to print the available list of
// actions in the console.
void
info()
{
    std::cout
            << "List of actions:\n"
            << "\t* q : quit application\n"
            << "\t* + : increment counter\n"
            << "\t* - : decrement counter\n"
            << std::endl;
}

// The reducer function returns a new state which is the old state
// incremented or decremented by one based on the action being sent.
// If the action doesn't equal "+" or "-" the old state
// is returned (with no changes).
state_type
reducer(state_type state, action_type action)
{
    if (action == "+")
        return state + 1;
    else if (action == "-")
        return state - 1;

    return state;
}

// The view prints the current state (the counter) in the console.
// Once subscribed to the store, this function will be called
// after an action is processed by the reducer function.
void
view(store_type const & store)
{
    std::cout << "State = " << store.state() << std::endl;
}

int
main()
{
    // A store is created with the reducer function
    // and an initial state (counter) of 0.
    store_type store{reducer, 0};
    // Then we subscribe to the store with the view function.
    // The subscribe function takes a "callable" with the
    // signature "void(void)" as an argument so a lambda (or any
    // other mean such as "std::bind") must be used since our
    // view function takes a store as an argument (instead of void).
    store.subscribe
    (
        [&store]()
        {
            view(store);
        }
    );

    // Print an initial info message followed by the initial count.
    info();
    view(store);

    // This while loop alternates between taking the user input
    //from the console and procesing it in the store
    while (true)
    {
        // Get the requested action from the console input
        action_type action;
        std::cin >> action;

        // If the user entered "q" then quit the application.
        if (action == "q")
            break;

        // If the action entered is recognized, then dispatch
        // it to the store.
        if (action == "+" || action == "-")
            store.dispatch(action);
        // Otherwise print a message noting that the input text
        // entered is not recognized, followed by the info text.
        else
        {
            std::cout << "Unknown action\n\n";
            info();
        }
    }

    return 0;
}
