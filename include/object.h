#pragma once

#include <string>
#include <functional>
#include <map>
#include <vector>

#define MO_OBJECT(NAME)                                  \
public:                                                  \
    virtual std::string GetType() const { return NAME; } \
    static std::string GetTypeStatic() { return NAME; }  \
    static size_t GetHashIDStatic() { return std::hash<std::string>{}(NAME); }

class Event;
class EventWrapper;

class Object
{
public:
    virtual std::string GetType() const
    {
        return "Object";
    }

    size_t GetHashID() const
    {
        return std::hash<std::string>{}(GetType());
    }

    static std::string GetTypeStatic()
    {
        return "Object";
    }

    void SendEvent(std::shared_ptr<Event> event);

    template <typename T>
    void RegisterEventListener(std::function<void(Object &, std::shared_ptr<Event>)> func)
    {
        size_t hashId = T::GetHashIDStatic();
        RegisterEventListener(hashId, func);
    }

    void RegisterEventListener(size_t hashId, std::function<void(Object &, std::shared_ptr<Event>)> func);

    template <typename T>
    void RemoveEventListener()
    {
        size_t hashId = T::GetHashIDStatic();
        RemoveEventListener(hashId);
    }

    void RemoveEventListener(size_t hashId);

    static std::map<size_t, std::vector<std::shared_ptr<EventWrapper>>> listeners;
};
