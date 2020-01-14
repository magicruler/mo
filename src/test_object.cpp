#include "test_object.h"
#include "event.h"

void TestObject::OnEvent(Event event)
{
    spdlog::info("xixi, {}", event.GetType());
}