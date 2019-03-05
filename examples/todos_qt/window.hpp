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

#ifndef EXAMPLE_WINDOW_HPP
#define EXAMPLE_WINDOW_HPP

#include "../todos/types.hpp"
#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <vector>

struct todo_widget : QWidget
{
    QCheckBox * check_box;
    QPushButton * button;

    todo_widget(QWidget * parent = nullptr);
};

struct window : QWidget
{
    window(store_type & store);
    void notify_state_change();

private:
    store_type & store;
    QWidget * todos_container;
    std::vector<todo_widget *> todo_widgets;
};

#endif // EXAMPLE_WINDOW_HPP
