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
#include "../models/Element.h"

namespace events {
    class Broker {
    public:
        Broker(const Broker&) = delete;
        Broker& operator=(const Broker&) = delete;

        // Static method to get the instance of the class.
        static Broker& getInstance() {
            static Broker instance;  // Guaranteed to be destroyed and instantiated on first use
            return instance;
        }

        void subscribe(ISubscriber *subscriber);

        void unsubscribe(ISubscriber *subscriber);

        void subscribe(const EventType &eventType, ISubscriber *subscriber);

        void unsubscribe(const EventType &eventType, ISubscriber *subscriber);

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

        std::map<EventType, std::list<ISubscriber *> > subscribers;
        std::unordered_set<ISubscriber*> generalSubscribers;
        std::queue<std::shared_ptr<Event> > eventQueue;
        std::mutex mutex;
        std::condition_variable cv;
        std::thread worker;
        std::atomic<bool> running;
        std::list<ISubscriber*> deferredRemovals;

        void processEvents();
    };
} // events

#endif //BROKER_H
