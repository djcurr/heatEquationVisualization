//
// Created by djcur on 4/18/2024.
//

#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <future>
#include <queue>
#include <thread>
#include <vector>
#include <functional>

namespace threads {
    /**
     * @class ThreadPool
     * @brief Manages a pool of threads to perform tasks asynchronously.
     *
     * This class allows for the concurrent execution of tasks using a fixed number of threads,
     * optimizing the use of multiple processors and reducing the overhead associated with thread creation and destruction.
     */
    class ThreadPool {
    public:
        /**
         * @brief Constructs a ThreadPool with a specified number of threads.
         * @param threads Number of threads to create in the pool.
         */
        explicit ThreadPool(size_t threads);

        /**
         * @brief Destroys the ThreadPool.
         *
         * Ensures all threads are properly joined before destruction to prevent any unfinished tasks.
         */
        ~ThreadPool();

        /**
         * @brief Adds a new task to the thread pool.
         * @tparam F Function type of the task.
         * @tparam Args Variadic template for the argument types.
         * @param f Function to be executed.
         * @param args Arguments to be passed to the function.
         * @return A future that will eventually hold the result of the function call.
         *
         * Tasks are enqueued and executed by available threads in the pool. If the pool has been stopped,
         * this method throws a runtime_error.
         */
        template<class F, class... Args>
        auto enqueue(F &&f, Args &&... args) -> std::future<std::result_of_t<F(Args...)>> {
            using return_type = std::result_of_t<F(Args...)>;

            auto task = std::make_shared<std::packaged_task<return_type()> >(
                std::bind(std::forward<F>(f), std::forward<Args>(args)...)
            );

            std::future<return_type> res = task->get_future(); {
                std::unique_lock<std::mutex> lock(queueMutex);
                if (stop)
                    throw std::runtime_error("enqueue on stopped ThreadPool");

                tasks.emplace([task]() { (*task)(); });
            }
            condition.notify_one();
            return res;
        }

    private:
        std::vector<std::thread> workers; ///< Threads that execute tasks.
        std::queue<std::function<void()> > tasks; ///< Queue of tasks to be executed.

        std::mutex queueMutex; ///< Mutex for protecting the task queue.
        std::condition_variable condition; ///< Condition variable to notify worker threads.
        bool stop = false; ///< Flag to stop the pool and no longer accept new tasks.
    };
} // threads

#endif //THREADPOOL_H
