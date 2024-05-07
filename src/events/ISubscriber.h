//
// Created by djcur on 4/20/2024.
//

#ifndef ISUBSCRIBER_H
#define ISUBSCRIBER_H

#include <memory>
#include "Event.h"

/** @namespace events
 *  @brief Encapsulates all components related to the event management system, including event dispatch and handling.
 */
namespace events {
    /**
     * @class ISubscriber
     * @brief Interface for objects that wish to subscribe to and handle events within the event management system.
     *
     * This interface requires implementing classes to handle events via the onEvent method.
     * Each subscriber must be capable of receiving and processing events passed to it by the event publisher, typically the Broker class.
     */
    class ISubscriber {
    public:
        /**
         * @brief Virtual destructor for ISubscriber.
         *
         * Ensures that derived classes can clean up resources safely when an ISubscriber pointer is deleted.
         */
        virtual ~ISubscriber() = default;

        /**
         * @brief Method called by the event dispatcher (Broker) when an event occurs.
         * @param event The event that has occurred, wrapped in a std::shared_ptr for memory management.
         *
         * Implement this method to handle incoming events. The exact response to the event will depend on the subscriber's role and the type of event received.
         */
        virtual void onEvent(const std::shared_ptr<Event>& event) = 0;
    };
}

#endif //ISUBSCRIBER_H
