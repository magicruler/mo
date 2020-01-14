#pragma once
#include "common.h"

class Event : public Object
{
    MO_OBJECT("Event")
};

class UpdateEvent: public Event
{
    MO_OBJECT("UpdateEvent")
};

class EventWrapper
{
public:
    EventWrapper(Object* target, std::function<void(Object &, std::shared_ptr<Event>)> func) : target(target),
                                                                                                               func(func)
    {
    }
    Object* target;
    std::function<void(Object &, std::shared_ptr<Event>)> func;
};