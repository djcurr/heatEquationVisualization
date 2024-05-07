//
// Created by djcur on 4/23/2024.
//

#ifndef THREADWRAPPER_H
#define THREADWRAPPER_H
#include <thread>

/**
 * @class ThreadWrapper
 * @brief Wraps std::thread to ensure threads are joined on destruction, preventing dangling threads.
 *
 * This class provides a wrapper around std::thread to manage the lifecycle of thread objects in a safe manner.
 * It ensures that each thread is joined before the ThreadWrapper object is destroyed, thereby avoiding any
 * potential for leaving unjoined threads that might execute beyond the lifetime of the objects they were intended to serve.
 */
class ThreadWrapper {
public:
    /**
     * @brief Default constructor.
     */
    ThreadWrapper() = default;

    /**
     * @brief Constructor that takes ownership of a std::thread object.
     * @param thread Rvalue reference to a std::thread to be managed by this wrapper.
     */
    explicit ThreadWrapper(std::thread&& thread)
        : m_thread(std::move(thread))
    {}

    /**
     * @brief Destructor that joins the managed thread, if it is joinable.
     */
    ~ThreadWrapper() {
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }

    // Disable copy semantics.
    ThreadWrapper(const ThreadWrapper&) = delete;
    ThreadWrapper& operator=(const ThreadWrapper&) = delete;

    /**
     * @brief Move constructor. Transfers ownership of a thread from another ThreadWrapper.
     * @param other Another ThreadWrapper to move from.
     */
    ThreadWrapper(ThreadWrapper&& other) noexcept
        : m_thread(std::move(other.m_thread))
    {}

    /**
     * @brief Move assignment operator. Transfers ownership of a thread from another ThreadWrapper.
     * @param other Another ThreadWrapper to move from.
     * @return Reference to this ThreadWrapper.
     *
     * If this ThreadWrapper already contains a joinable thread, it is joined before transferring ownership.
     */
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
    std::thread m_thread; ///< Managed std::thread object.
};

#endif //THREADWRAPPER_H
