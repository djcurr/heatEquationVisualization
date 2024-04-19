//
// Created by djcur on 4/18/2024.
//

#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <future>
#include <queue>
#include <thread>
#include <vector>

namespace threads {
    class ThreadPool {
    public:
        explicit ThreadPool(size_t threads);
        ~ThreadPool();
        template<class F, class... Args>
        auto enqueue(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
            using return_type = typename std::result_of<F(Args...)>::type;

            auto task = std::make_shared<std::packaged_task<return_type()>>(
                std::bind(std::forward<F>(f), std::forward<Args>(args)...)
            );

            std::future<return_type> res = task->get_future();
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                if(stop)
                    throw std::runtime_error("enqueue on stopped ThreadPool");

                tasks.emplace([task](){ (*task)(); });
            }
            condition.notify_one();
            return res;
        }

    private:
        std::vector<std::thread> workers;
        std::queue<std::function<void()> > tasks;

        std::mutex queueMutex;
        std::condition_variable condition;
        bool stop = false;
    };
} // threads

#endif //THREADPOOL_H
