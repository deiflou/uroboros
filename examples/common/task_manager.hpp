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

#ifndef UROBOROS_EXAMPLES_TASK_MANAGER_HPP
#define UROBOROS_EXAMPLES_TASK_MANAGER_HPP

#include <deque>
#include <mutex>
#include <functional>
#include <utility>
#include <vector>
#include <atomic>
#include <stlab/concurrency/task.hpp>

namespace uroboros::examples
{

struct task_manager
{
    using task_type = stlab::task<void()>;
    using queue_type = std::deque<task_type>;

    task_manager(task_manager const &) = delete;
    task_manager & operator=(task_manager const &) = delete;

    static
    task_manager &
    instance()
    {
        static task_manager instance_;
        return instance_;
    }

    void
    add_task(task_type && task)
    {
        std::lock_guard<std::mutex> lock_guard(task_queue_mutex_);
        task_queue_.emplace_back(std::move(task));
    }

    bool
    execute_next_pending_task()
    {
        task_type task;

        {
            std::lock_guard<std::mutex> lock_guard(task_queue_mutex_);

            if (task_queue_.empty())
                return false;

            task = task_queue_.front();
            task_queue_.pop_front();
        }

        task();
    }

    void
    execute_all_pending_tasks()
    {
        queue_type tasks;
        
        {
            std::lock_guard<std::mutex> lock_guard(task_queue_mutex_);
            std::swap(task_queue_, tasks);
        }
        
        for (task_type & task : tasks)
            task();
    }

    void
    clear_tasks()
    {
        std::lock_guard<std::mutex> lock_guard(task_queue_mutex_);
        task_queue_.clear();
    }

private:
    queue_type task_queue_{};
    std::mutex task_queue_mutex_{};

    task_manager() = default;
};

}

#endif // UROBOROS_EXAMPLES_TASK_MANAGER_HPP
