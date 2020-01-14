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
