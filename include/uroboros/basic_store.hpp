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

#ifndef UROBOROS_BASIC_STORE_HPP
#define UROBOROS_BASIC_STORE_HPP

#include "detail/scope_guard.hpp"
#include <unordered_map>
#include <stdexcept>

namespace uroboros
{

template
<
    typename State_Type,
    typename Action_Type,
    template <typename...> class Function_Type = std::function
>
struct basic_store
{
    using state_type = State_Type;
    using action_type = Action_Type;
    template <typename Signature_Type>
    using function_type = Function_Type<Signature_Type>;
    using reducer_type =
        function_type<state_type(state_type, action_type)>;
    using listener_type = function_type<void()>;
    using unsubscriber_type = function_type<void()>;

private:
    using listener_collection =
        std::unordered_map<unsigned int, listener_type>;

public:
    basic_store(reducer_type reducer, state_type state = {})
    {
        if (!reducer)
            throw
                std::runtime_error
                (
                    "The reducer must be a non empty function."
                );

        reducer_ = reducer;
        state_ = state;
    }

    virtual
    ~basic_store()
    {}

    virtual
    state_type
    state() const
    {
        if (is_dispatching_)
            throw
                std::runtime_error
                (
                    "store::state() can not be called while "
                    "the reducer is executing.\n"
                    "The state has already been passed as "
                    "a parameter to the reducer.\n"
                    "This parameter must be used instead tho get the state."
                );

        return state_;
    }

    virtual
    unsubscriber_type
    subscribe(listener_type listener)
    {
        if (!listener)
            throw
                std::runtime_error
                (
                    "The listener must be a non empty function."
                );

        if (is_dispatching_)
            throw
                std::runtime_error
                (
                    "store::subscribe() can not be called while "
                    "the reducer is executing."
                );

        static unsigned int next_listener_id{0};

        listeners_[next_listener_id] = listener;

        return
            [id = next_listener_id++, this]()
            {
                auto listener_iterator = this->listeners_.find(id);
                if (listener_iterator == this->listeners_.end())
                    return;

                if (this->is_dispatching_)
                    throw
                        std::runtime_error
                        (
                            "The unsubscriber function can not be "
                            "called while the reducer is executing."
                        );

                this->listeners_.erase(listener_iterator);
            };
    }

    virtual
    action_type
    dispatch(action_type action)
    {
        auto a = reduce(action);
        notify();
        return a;
    }

    virtual
    void
    replace_reducer(reducer_type reducer)
    {
        if (!reducer)
            throw
                std::runtime_error
                (
                    "The reducer must be a non empty function."
                );

        reducer_ = reducer;
    }

protected:
    virtual
    action_type
    reduce(action_type action)
    {
        if (is_dispatching_)
            throw
                std::runtime_error
                (
                    "You may not dispatch actions while "
                    "the reducer is executing."
                );

        {
            detail::scope_guard guard
            (
                [this]()
                {
                    this->is_dispatching_ = false;
                }
            );

            is_dispatching_ = true;
            state_ = reducer_(state_, action);
        }
        return action;
    }

    virtual
    void
    notify()
    {
        auto current_listeners = listeners_;

        for (auto & listener : current_listeners)
            listener.second();
    }

private:
    reducer_type reducer_;
    state_type state_;
    listener_collection listeners_;
    bool is_dispatching_{false};
    
};

}

#endif // UROBOROS_BASIC_STORE_HPP
