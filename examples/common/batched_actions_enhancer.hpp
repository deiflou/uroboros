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

#ifndef UROBOROS_EXAMPLES_BATCHED_ACTIONS_ENHANCER_HPP
#define UROBOROS_EXAMPLES_BATCHED_ACTIONS_ENHANCER_HPP

#include <vector>

namespace uroboros::examples
{

template <typename Base_Type>
struct batched_actions_enhancer : public Base_Type
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
    
    using action_vector_type = std::vector<action_type>;

    virtual
    action_vector_type
    dispatch(action_vector_type actions)
    {
        for (action_type const & action : actions)
            this->reduce(action);

        this->notify();

        return actions;
    }

};

}

#endif // UROBOROS_EXAMPLES_BATCHED_ACTIONS_ENHANCER_HPP
