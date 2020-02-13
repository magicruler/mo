#pragma once
#include "common.h"

class UpdateEvent: public Event
{
    MO_EVENT("UpdateEvent")
};

class SelectionPropertyChangeEvent : public Event
{
    MO_EVENT("SelectionPropertyChangeEvent")
};
