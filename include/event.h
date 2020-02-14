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

class DebugMessageEvent :public Event
{
    MO_EVENT("DebugMessageEvent")
public:
    std::string msg;
};