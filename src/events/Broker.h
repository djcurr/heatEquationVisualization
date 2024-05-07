//
// Created by djcur on 4/20/2024.
//

#ifndef BROKER_H
#define BROKER_H

#include "ISubscriber.h"
#include <list>
#include <map>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_set>

/** @namespace events
 *  @brief Encapsulates classes related to the event management system.
 */
namespace events {
    /** @class Broker
     *  @brief Manages event distribution among subscribers.
     *
     *  Handles the subscription and unsubscription of event listeners and dispatches events to them.
     *  Implemented as a singleton to ensure a single centralized event manager.
     */
    class Broker {
    public:
        Broker(const Broker &) = delete;

        Broker &operator=(const Broker &) = delete;

        /** @brief Access the singleton instance of the Broker.
         *  @return Reference to the singleton Broker instance.
         */
        static Broker &getInstance() {
            static Broker instance; // Guaranteed to be destroyed and instantiated on first use
            return instance;
        }

        /** @brief Subscribe a subscriber to all events.
         *  @param subscriber Pointer to the subscriber.
         */
        void subscribe(ISubscriber *subscriber);

        /** @brief Unsubscribe a subscriber from all events.
         *  @param subscriber Pointer to the subscriber.
         */
        void unsubscribe(ISubscriber *subscriber);

        /** @brief Subscribe a subscriber to a specific event type.
         *  @param eventTypes Type of event.
         *  @param subscriber Pointer to the subscriber.
         */
        void subscribe(const std::vector<EventType> &eventTypes, ISubscriber *subscriber);

        /** @brief Publish an event to all subscribed listeners.
         *  @param args Arguments used to construct the event.
         */
        template<typename EventType, typename... Args>
        void publish(Args &&... args) {
            auto event = std::make_shared<EventType>(std::forward<Args>(args)...); {
                std::lock_guard<std::mutex> lock(mutex);
                eventQueue.push(event);
            }
            cv.notify_one();
        }

    private:
        Broker() : running(true) {
            worker = std::thread(&Broker::processEvents, this);
        }

        ~Broker() {
            // Stop the event processing thread
            running = false;
            cv.notify_one();
            if (worker.joinable()) {
                worker.join();
            }
        }

        /** @brief Processes events queued in eventQueue.
         *  Runs in a separate thread to handle event distribution to subscribers.
         */
        void processEvents();

        std::map<EventType, std::list<ISubscriber *> > subscribers; ///< Maps event types to subscribers.
        std::unordered_set<ISubscriber *> generalSubscribers; ///< Subscribers that receive all events.
        std::queue<std::shared_ptr<Event> > eventQueue; ///< Queue of events to be distributed.
        std::mutex mutex; ///< Mutex to synchronize access to the event queue.
        std::condition_variable cv; ///< Condition variable for notifying event processing.
        std::thread worker; ///< Thread dedicated to processing events.
        std::atomic<bool> running; ///< Flag to control the running state of the event processor.
        std::list<ISubscriber *> deferredRemovals; ///< Subscribers queued for removal after processing.
    };
} // namespace events

#endif //BROKER_H
