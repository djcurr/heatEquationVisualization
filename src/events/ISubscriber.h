//
// Created by djcur on 4/20/2024.
//

#ifndef ISUBSCRIBER_H
#define ISUBSCRIBER_H
#include <memory>

#include "Event.h"

namespace events {
    class ISubscriber {
    public:
        virtual ~ISubscriber() = default;
        virtual void onEvent(const std::shared_ptr<Event>& event) = 0;
    };
}
#endif //ISUBSCRIBER_H
