
.. highlight:: cpp

.. image:: resources/logo_gradient_text_right.svg
   :align: center
   :alt: Uroboros logo
   :width: 300

**Uroboros** is a c++ library based on redux_.

*Extended readme and documentation coming soon.*

Example
=======

The following example is a simple console application that lets you
increment or decrement a counter. It is included in the examples directory.

In the console you can type :kbd:`+`, :kbd:`-` or :kbd:`q` and hit :kbd:`Enter`
to send an increment or decrement action to the store, or quit the application
respectively. Any other sequence of caracters introduced will print an error
message followed by an info text with the allowed actions.

::

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

.. _redux: https://redux.js.org
