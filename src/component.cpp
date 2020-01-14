#include "component.h"

unsigned int Component::IdCounter = 0;
Component::Component()
{
    IdCounter++;
    ID = IdCounter;
}