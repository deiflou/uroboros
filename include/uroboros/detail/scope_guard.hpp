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

#ifndef UROBOROS_DETAIL_SCOPE_GUARD_HPP
#define UROBOROS_DETAIL_SCOPE_GUARD_HPP

#include <functional>

namespace uroboros::detail
{

struct scope_guard final
{
    scope_guard(std::function<void()> && f) :
        f_(std::forward<std::function<void()>>(f))
    {}

    scope_guard(scope_guard && other) :
        f_(std::move(other.f_))
    {
        other.f_ = nullptr;
    }

    ~scope_guard()
    {
        // f must not throw
        if (f_)
            f_();
    }

    void dismiss() noexcept
    {
        f_ = nullptr;
    }

    scope_guard(scope_guard const &) = delete;
    void operator=(scope_guard const &) = delete;

private:
    std::function<void()> f_;

};

}

#endif // UROBOROS_DETAIL_SCOPE_GUARD_HPP
