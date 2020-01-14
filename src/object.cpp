#include "object.h"
#include "event.h"

std::map<size_t, std::vector<std::shared_ptr<EventWrapper>>> Object::listeners = std::map<size_t, std::vector<std::shared_ptr<EventWrapper>>>();

void Object::SendEvent(std::shared_ptr<Event> event)
{
    size_t hashId = event->GetHashID();
    auto it = listeners.find(hashId);
    if (it != listeners.end())
    {
        for (auto &item : it->second)
        {
            item->func(*item->target, event);
        }
    }
}

void Object::RegisterEventListener(size_t hashId, std::function<void(Object &, std::shared_ptr<Event>)> func)
{
    auto eventWrapper = std::make_shared<EventWrapper>(this, func);

    auto it = listeners.find(hashId);
    if (it != listeners.end())
    {
        it->second.push_back(eventWrapper);
    }
    else
    {
        listeners[hashId] = std::vector<std::shared_ptr<EventWrapper>>();
        listeners[hashId].push_back(eventWrapper);
    }
}

void Object::RemoveEventListener(size_t hashId)
{
    auto it = listeners.find(hashId);
    if (it != listeners.end())
    {
        for (size_t i = it->second.size() - 1; i >= 0; i--)
        {
            if (it->second[i]->target == this)
            {
                it->second.erase(it->second.begin() + i);
            }
        }
    }
}