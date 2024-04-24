//
// Created by djcur on 4/23/2024.
//

#ifndef THREADWRAPPER_H
#define THREADWRAPPER_H
#include <thread>

class ThreadWrapper {
public:
    ThreadWrapper() = default;

    explicit ThreadWrapper(std::thread&& thread)
        : m_thread(std::move(thread))
    {}

    ~ThreadWrapper() {
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }

    ThreadWrapper(const ThreadWrapper&) = delete;
    ThreadWrapper& operator=(const ThreadWrapper&) = delete;

    ThreadWrapper(ThreadWrapper&& other) noexcept
        : m_thread(std::move(other.m_thread))
    {}

    ThreadWrapper& operator=(ThreadWrapper&& other) noexcept {
        if (this != &other) {
            if (m_thread.joinable()) {
                m_thread.join();
            }
            m_thread = std::move(other.m_thread);
        }
        return *this;
    }

private:
    std::thread m_thread;
};

#endif //THREADWRAPPER_H
