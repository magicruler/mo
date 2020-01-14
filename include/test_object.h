#pragma once
#include "common.h"

class TestObject : public Object
{
    MO_OBJECT("TestObject")
public:
    virtual void OnEvent(Event event);
};