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

#ifndef UROBOROS_STORE_HPP
#define UROBOROS_STORE_HPP

#include "basic_store.hpp"
#include "detail/store_enhancer.hpp"
#include <functional>

namespace uroboros
{

template
<
    typename State_Type,
    typename Action_Type,
    template <typename...> class Function_Type = std::function
>
using store = basic_store<State_Type, Action_Type, Function_Type>;

template
<
    typename Base_Store_Type,
    template <typename> class ... Enhancers
>
using enhanced_store =
    typename detail::store_enhancer<Base_Store_Type, Enhancers...>::type;

}

#endif // UROBOROS_STORE_HPP
