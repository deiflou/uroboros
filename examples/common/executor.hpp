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

#ifndef UROBOROS_EXAMPLES_EXECUTOR_HPP
#define UROBOROS_EXAMPLES_EXECUTOR_HPP

#if defined(QT_CORE_LIB) && !defined(STLAB_DISABLE_QT_MAIN_EXECUTOR)

#include <stlab/concurrency/main_executor.hpp>
constexpr auto & executor = stlab::main_executor;

#else

#include "task_manager.hpp"

namespace uroboros::examples
{
    
struct executor_type {
    using result_type = void;

    template <typename F>
    void
    operator()(F f) const
    {
        task_manager::instance().add_task(std::move(f));
    }
};

constexpr auto executor = executor_type{};

}

#endif

#endif // UROBOROS_EXAMPLES_EXECUTOR_HPP
