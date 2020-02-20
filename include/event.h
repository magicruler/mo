#pragma once
#include "common.h"

class RenderTarget;

class RenderTargetResizeEvent : public Event
{
    MO_EVENT("RenderTargetResizeEvent")
public:
    RenderTarget* renderTarget = nullptr;
    glm::vec2 size;
};

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