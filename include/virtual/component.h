#pragma once

#include <memory>
#include <vector>

#include "util/definitions.h"
#include "event_system/Subscriber.h"
#include "render/actor_2d.h"

class Actor2D;

class Component {
protected:
    std::vector<Subscriber*> subscribers;

public:
    // virtual Component() = 0;
    // virtual ~Component() = 0;
    virtual void Update(std::shared_ptr<void> delta_time) = 0;
};
