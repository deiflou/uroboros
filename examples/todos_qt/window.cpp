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

#include "window.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QScrollArea>
#include <QComboBox>
#include <algorithm>
#include <utility>

todo_widget::todo_widget(QWidget * parent) :
    QWidget(parent)
{
    QHBoxLayout * horizontal_layout = new QHBoxLayout;
    check_box = new QCheckBox;
    button = new QPushButton("Remove");

    horizontal_layout->setMargin(0);
    horizontal_layout->setSpacing(5);

    horizontal_layout->addWidget(check_box, 1);
    horizontal_layout->addWidget(button);

    setLayout(horizontal_layout);
}

window::window(store_type & store) :
    store(store)
{
    QVBoxLayout * vertical_layout = new QVBoxLayout;
    QHBoxLayout * horizontal_layout = new QHBoxLayout;
    QPushButton * button = new QPushButton("Add Todo");
    QLineEdit * line_edit = new QLineEdit;
    QComboBox * combo_box = new QComboBox;
    QScrollArea * scroll_area = new QScrollArea;
    todos_container = new QWidget;

    setMinimumSize(500, 400);

    vertical_layout->setMargin(0);
    vertical_layout->setSpacing(0);
    horizontal_layout->setMargin(10);
    horizontal_layout->setSpacing(5);
    line_edit->setPlaceholderText("Insert TODO text here");
    combo_box->addItems({"Show all", "Show completed", "Show active"});
    scroll_area->setWidget(todos_container);
    scroll_area->setWidgetResizable(true);
    scroll_area->setFrameStyle(QScrollArea::NoFrame);
    todos_container->setLayout(new QVBoxLayout);
    todos_container->layout()->setAlignment(Qt::AlignTop);
    todos_container->layout()->setMargin(10);
    todos_container->layout()->setSpacing(5);

    horizontal_layout->addWidget(line_edit);
    horizontal_layout->addWidget(button);
    horizontal_layout->addWidget(combo_box);

    vertical_layout->addLayout(horizontal_layout, 0);
    vertical_layout->addWidget(scroll_area, 1);

    setLayout(vertical_layout);

    connect
    (
        button,
        &QPushButton::clicked,
        [&store, line_edit]()
        {
            store.dispatch(add_todo_action_type{line_edit->text().toStdString()});
        }
    );

    connect
    (
        combo_box,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        [&store](int index)
        {
            visibility_filter_type filter;
            if (index == 0)
                filter = visibility_filter_type::show_all;
            else if (index == 1)
                filter = visibility_filter_type::show_completed;
            else
                filter = visibility_filter_type::show_active;
            store.dispatch(set_visibility_action_type{filter});
        }
    );
}

void
window::notify_state_change()
{
    std::vector<std::pair<int, todo_type>> selected_todos;

    if (store.state().visibility_filter == visibility_filter_type::show_all)
        for (int i = 0; i < store.state().todos.size(); ++i)
            selected_todos.push_back({i, store.state().todos[i]});
    else if (store.state().visibility_filter == visibility_filter_type::show_completed)
        for (int i = 0; i < store.state().todos.size(); ++i)
        {
            if (store.state().todos[i].is_completed)
                selected_todos.push_back({i, store.state().todos[i]});
        }
    else
        for (int i = 0; i < store.state().todos.size(); ++i)
        {
            if (!store.state().todos[i].is_completed)
                selected_todos.push_back({i, store.state().todos[i]});
        }

    int const size1 = selected_todos.size();
    int const size2 = todo_widgets.size();
    int diff = size1 - size2;
    
    if (diff > 0)
        for (int i = size2; i < size1; ++i)
        {
            todo_widget * widget = new todo_widget(todos_container);
            todos_container->layout()->addWidget(widget);
            todo_widgets.push_back(widget);
            widget->show();
        }
    else if (diff < 0)
        while (diff++)
        {
            todo_widget * widget = todo_widgets.back();
            widget->setParent(nullptr);
            delete widget;
            todo_widgets.pop_back();
        }
    else
        if (size1 == 0)
            return;

    for (int i = 0; i < size1; ++i)
    {
        todo_widgets[i]->check_box->disconnect();
        todo_widgets[i]->button->disconnect();
        
        todo_widgets[i]->check_box->setText
        (
            selected_todos[i].second.text.c_str()
        );
        todo_widgets[i]->check_box->setChecked
        (
            selected_todos[i].second.is_completed
        );
        
        connect
        (
            todo_widgets[i]->check_box,
            &QCheckBox::toggled,
            [this, index = selected_todos[i].first]()
            {
                store.dispatch(toggle_todo_action_type{index});
            }
        );
        connect
        (
            todo_widgets[i]->button,
            &QPushButton::clicked,
            [this, index = selected_todos[i].first]()
            {
                store.dispatch(delete_todo_action_type{index});
            }
        );
    }
}
