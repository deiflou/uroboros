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

#include "types.hpp"
#include "window.hpp"
#include <QApplication>
#include <iostream>

extern
state_type
reducer(state_type, action_type);

extern
dispatch_transformer_type
async_middleware(middleware_api_type api);

int
main(int argc, char *argv[])
{
    store_type store(reducer, {});

    store.apply_middleware(async_middleware);

    QApplication app(argc, argv);
    window wnd(store);

    auto unsubscribe_function =
        store.subscribe
        (
            [&wnd]()
            {
                wnd.notify_state_change();
            }
        );

    wnd.show();
    auto ret = app.exec();

    unsubscribe_function();
    
    std::cout << "Waiting for the active tasks to finish..." << std::endl;

    for (auto const & async_task_info : store.state().async_tasks)
        store.dispatch(cancel_async_task_action_type{async_task_info.id});
        
    while (store.state().async_tasks.size())
        QApplication::processEvents();

    return ret;
}
