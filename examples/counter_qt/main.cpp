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

#include <uroboros/store.hpp>
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <string>

using state_type = int;
using action_type = std::string;
using store_type = uroboros::store<state_type, action_type>;

struct gui_type
{
    QWidget * main_window{nullptr};
    QLabel * counter_text_label{nullptr};
    QLabel * counter_label{nullptr};
    QPushButton * increment_button{nullptr};
    QPushButton * decrement_button{nullptr};

    gui_type()
    {
        main_window = new QWidget;
        counter_text_label = new QLabel("Count: ");
        counter_label = new QLabel;
        increment_button = new QPushButton("Increment Counter");
        decrement_button = new QPushButton("Decrement Counter");
        QVBoxLayout * main_layout = new QVBoxLayout;
        QHBoxLayout * counter_layout = new QHBoxLayout;

        QFont font = counter_label->font();
        font.setPointSize(18);
        counter_text_label->setFont(font);
        counter_label->setFont(font);

        counter_layout->addWidget(counter_text_label);
        counter_layout->addWidget(counter_label);
        counter_layout->addStretch();
        main_layout->addLayout(counter_layout);
        main_layout->addWidget(increment_button);
        main_layout->addWidget(decrement_button);
        main_layout->addStretch();

        main_window->setLayout(main_layout);
    }
};

state_type
reducer(state_type state, action_type action)
{
    if (action == "+")
        return state + 1;
    else if (action == "-")
        return state - 1;

    return state;
}

void
update_gui(store_type const & store, gui_type const & gui)
{
    gui.counter_label->setText(QString::number(store.state()));
}

int
main(int argc, char ** argv)
{
    QApplication app(argc, argv);
    gui_type gui;
    store_type store(reducer, 0);

    QApplication::connect
    (
        gui.increment_button,
        &QPushButton::clicked,
        [&store]()
        {
            store.dispatch("+");
        }
    );
    QApplication::connect
    (
        gui.decrement_button,
        &QPushButton::clicked,
        [&store]()
        {
            store.dispatch("-");
        }
    );

    store.subscribe
    (
        [&store, &gui]()
        {
            update_gui(store, gui);
        }
    );

    update_gui(store, gui);
    
    gui.main_window->show();

    return app.exec();
}
