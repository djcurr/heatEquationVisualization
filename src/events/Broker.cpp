//
// Created by djcur on 4/20/2024.
//

#include "Broker.h"

#include <iostream>

namespace events {
    void Broker::subscribe(ISubscriber *subscriber) {
        std::lock_guard<std::mutex> lock(mutex);
        generalSubscribers.insert(subscriber);
    }

    void Broker::unsubscribe(ISubscriber *subscriber) {
        std::lock_guard<std::mutex> lock(mutex);
        deferredRemovals.push_back(subscriber);
    }

    void Broker::subscribe(const EventType &eventType, ISubscriber *subscriber) {
        std::lock_guard<std::mutex> lock(mutex);
        auto &subscriberList = subscribers[eventType];
        if (std::find(subscriberList.begin(), subscriberList.end(), subscriber) == subscriberList.end()) {
            subscriberList.push_back(subscriber);
        }
    }

    void Broker::unsubscribe(const EventType &eventType, ISubscriber *subscriber) {
        std::lock_guard<std::mutex> lock(mutex);
        auto &subs = subscribers[eventType];
        subs.remove(subscriber);
    }

    void Broker::processEvents() {
        while (running) {
            std::queue<std::shared_ptr<Event>> localQueue;
            {
                std::unique_lock<std::mutex> lock(mutex);
                cv.wait(lock, [this] { return !eventQueue.empty() || !running; });
                std::swap(localQueue, eventQueue);
            }

            while (!localQueue.empty()) {
                auto event = localQueue.front();
                localQueue.pop();
                // std::cout << getEventTypeName(event->getType());
                // Copy the subscribers to a local list to avoid holding the lock while calling onEvent
                std::list<ISubscriber*> localSpecificSubs;
                std::unordered_set<ISubscriber*> localGeneralSubs;
                {
                    std::lock_guard<std::mutex> lock(mutex);
                    localSpecificSubs = subscribers[event->getType()];
                    localGeneralSubs = generalSubscribers;
                }

                // Notify specific subscribers
                for (auto* subscriber : localSpecificSubs) {
                    if (subscriber != nullptr && localGeneralSubs.find(subscriber) == localGeneralSubs.end()) {
                        subscriber->onEvent(event);
                    }
                }

                // Notify general subscribers
                for (auto* subscriber : localGeneralSubs) {
                    if (subscriber != nullptr) subscriber->onEvent(event);
                }
            }
        }

        // In processEvents(), after notifying all subscribers, handle deferred removals
        {
            std::lock_guard<std::mutex> lock(mutex);
            for (auto* subscriber : deferredRemovals) {
                generalSubscribers.erase(subscriber);
                // Also, remove from specific subscriber lists if necessary
                for (auto& pair : subscribers) {
                    pair.second.remove(subscriber);
                }
            }
            deferredRemovals.clear();
        }
    }
} // events
