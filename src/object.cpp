#include "object.h"
#include "event.h"

std::map<size_t, std::vector<std::shared_ptr<Object>>> Object::listeners = std::map<size_t, std::vector<std::shared_ptr<Object>>>();

void Object::SendEvent(std::shared_ptr<Event> event)
{
    size_t hashId = event->GetHashID();
    auto it = listeners.find(hashId);
    if (it != listeners.end())
    {
        for (auto &item : it->second)
        {
            item->OnEvent(event);
        }
    }
}

void Object::RegisterEventListener(size_t hashId)
{
    auto it = listeners.find(hashId);
    if (it != listeners.end())
    {
        it->second.push_back(shared_from_this());
    }
    else
    {
        listeners[hashId] = std::vector<std::shared_ptr<Object>>();
        listeners[hashId].push_back(shared_from_this());
    }
}

void Object::RemoveEventListener(size_t hashId)
{
    auto it = listeners.find(hashId);
    if (it != listeners.end())
    {
        for (size_t i = it->second.size() - 1; i >= 0; i--)
        {
            if (it->second[i].get() == this)
            {
                it->second.erase(it->second.begin() + i);
            }
        }
    }
}