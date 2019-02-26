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

#ifndef UROBOROS_MIDDLEWARE_ENHANCER_HPP
#define UROBOROS_MIDDLEWARE_ENHANCER_HPP

#include <functional>
#include <vector>
#include <algorithm>
#include <numeric>
#include <stdexcept>

namespace uroboros
{

template <typename Base_Type>
struct middleware_enhancer : public Base_Type
{
    using typename Base_Type::reducer_type;
    using typename Base_Type::state_type;
    using typename Base_Type::action_type;
    template <typename Signature_Type>
    using function_type =
        typename Base_Type::template function_type<Signature_Type>;
    using typename Base_Type::listener_type;
    using typename Base_Type::unsubscriber_type;

    using Base_Type::Base_Type;
    using Base_Type::dispatch;

    using state_function_type = function_type<state_type(void)>;
    using dispatch_function_type = function_type<action_type(action_type)>;

    struct middleware_api final
    {
        state_function_type state;
        dispatch_function_type dispatch;
    };
    
    using dispatch_transformer_type =
                function_type<dispatch_function_type(dispatch_function_type)>;
    using middleware_type =
                function_type<dispatch_transformer_type(middleware_api)>;

    virtual
    action_type
    dispatch(action_type action) override
    {
        return reducer_(action);
    }

    void
    apply_middleware()
    {
        reducer_ = base_reducer_;
    }

    template <typename... Middlewares_Types>
    void
    apply_middleware(Middlewares_Types... middlewares)
    {
        reducer_ =
            [](action_type) -> action_type
            {
                throw std::runtime_error
                (
                    "You may not dispatch actions while "
                    "the middleware is been constructed."
                );
            };

        std::vector<middleware_type> middleware_vector{middlewares...};
        std::vector<dispatch_transformer_type> dispatch_transformer_vector;

        std::transform
        (
            middleware_vector.begin(), middleware_vector.end(),
            std::back_inserter(dispatch_transformer_vector),
            [this](middleware_type middleware) -> dispatch_transformer_type
            {
                return middleware(middleware_api_);
            }
        );

        dispatch_transformer_type initial_dispatch_transformer
        {
            [](dispatch_function_type dispatch_function)
            -> dispatch_function_type
            {
                return dispatch_function;
            }
        };

        reducer_ =
            std::accumulate
            (
                dispatch_transformer_vector.begin(),
                dispatch_transformer_vector.end(),
                initial_dispatch_transformer,
                [](dispatch_transformer_type dispatch_transformer_a,
                   dispatch_transformer_type dispatch_transformer_b)
                ->dispatch_transformer_type
                {
                    return
                        [dispatch_transformer_a, dispatch_transformer_b]
                        (dispatch_function_type dispatch_function)
                        -> dispatch_function_type
                        {
                            return
                                dispatch_transformer_a
                                (
                                    dispatch_transformer_b
                                    (
                                        dispatch_function
                                    )
                                );
                        };
                }
            )
            (base_reducer_);
    }

private:
    dispatch_function_type base_reducer_
    {
        [this](action_type action) -> action_type
        {
            return Base_Type::dispatch(action);
        }
    };
    dispatch_function_type reducer_{base_reducer_};
    middleware_api middleware_api_
    {
        [this]() -> state_type
        {
            // return Base_Type::state();
            return this->state();
        },
        [this](action_type action) -> action_type
        {
            // action_type a = this->reduce(action);
            // this->notify();
            // return a;
            return this->dispatch(action);
        }
    };
};

}

#endif // UROBOROS_MIDDLEWARE_ENHANCER_HPP
