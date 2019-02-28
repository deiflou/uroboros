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
#include <QSpinBox>
#include <QScrollArea>
#include <iostream>

progress_widget::progress_widget(QWidget * parent) :
    QWidget(parent)
{
    QVBoxLayout * vertical_layout = new QVBoxLayout;
    QHBoxLayout * horizontal_layout = new QHBoxLayout;
    label = new QLabel;
    progress_bar = new QProgressBar;
    button = new QPushButton("Cancel");

    vertical_layout->setMargin(0);
    vertical_layout->setSpacing(5);
    horizontal_layout->setMargin(0);
    horizontal_layout->setSpacing(5);
    progress_bar->setRange(0, 100);

    horizontal_layout->addWidget(progress_bar);
    horizontal_layout->addWidget(button);

    vertical_layout->addWidget(label);
    vertical_layout->addLayout(horizontal_layout);

    setLayout(vertical_layout);
}

window::window(store_type & store) :
    store(store)
{
    QVBoxLayout * vertical_layout = new QVBoxLayout;
    QHBoxLayout * horizontal_layout = new QHBoxLayout;
    QPushButton * button = new QPushButton("Add Task");
    QSpinBox * spin = new QSpinBox;
    QScrollArea * scroll_area = new QScrollArea;
    progress_container = new QWidget;

    setMinimumSize(500, 400);

    vertical_layout->setMargin(0);
    vertical_layout->setSpacing(0);
    horizontal_layout->setMargin(10);
    horizontal_layout->setSpacing(5);
    spin->setRange(1, 10);
    spin->setValue(5);
    spin->setSuffix(" seconds");
    scroll_area->setWidget(progress_container);
    scroll_area->setWidgetResizable(true);
    progress_container->setLayout(new QVBoxLayout);
    progress_container->layout()->setAlignment(Qt::AlignTop);
    progress_container->layout()->setMargin(10);
    progress_container->layout()->setSpacing(5);

    horizontal_layout->addWidget(button);
    horizontal_layout->addWidget(spin);

    vertical_layout->addLayout(horizontal_layout);
    vertical_layout->addWidget(scroll_area);

    setLayout(vertical_layout);

    connect
    (
        button,
        &QPushButton::clicked,
        [&store, spin]()
        {
            store.dispatch(start_async_task_action_type{spin->value()});
        }
    );
}

void
window::notify_state_change()
{
    int const size1 = store.state().async_tasks.size();
    int const size2 = progress_widgets.size();
    int diff = size1 - size2;
    
    if (diff > 0)
        for (int i = 0; i < diff; ++i)
        {
            progress_widget * widget = new progress_widget(progress_container);
            progress_container->layout()->addWidget(widget);
            progress_widgets.push_back(widget);
            widget->show();
        }
    else if (diff < 0)
        while (diff++)
        {
            progress_widget * widget = progress_widgets.back();
            widget->setParent(nullptr);
            delete widget;
            progress_widgets.pop_back();
        }
    else
        if (size1 == 0)
            return;

    for (int i = 0; i < size1; ++i)
    {
        progress_widgets[i]->label->setText
        (
            QString("Task id: ") +
            QString::number(store.state().async_tasks[i].id)
        );
        progress_widgets[i]->progress_bar->setValue
        (
            store.state().async_tasks[i].progress
        );
    }

    if (diff != 0)
        for (int i = 0; i < size1; ++i)
        {
            int task_id = store.state().async_tasks[i].id;
            progress_widgets[i]->button->disconnect();
            connect
            (
                progress_widgets[i]->button,
                &QPushButton::clicked,
                [this, task_id]()
                {
                    store.dispatch(cancel_async_task_action_type{task_id});
                }
            );
        }

}
