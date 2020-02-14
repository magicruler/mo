#pragma once
#include <string>
#include <functional>
#include <map>
#include <vector>
#include <memory>
#include <list>

#define MO_OBJECT(NAME)                                  \
public:                                                  \
    virtual std::string GetType() const { return NAME; } \
    static std::string GetTypeStatic() { return NAME; }  \
    virtual size_t GetHashID() { return std::hash<std::string>{}(NAME); }\
    static size_t GetHashIDStatic() { return std::hash<std::string>{}(NAME); }

#define MO_EVENT(NAME)                                  \
public:                                                  \
    virtual std::string GetTypeName() const { return NAME; } \
    static std::string GetTypeNameStatic() { return NAME; }  \
    virtual size_t GetHashID() const { return std::hash<std::string>{}(NAME); }\
    static size_t GetHashIDStatic() { return std::hash<std::string>{}(NAME); }

class Event
{
    MO_EVENT("Event")
};

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

    void Register(size_t eventId)
    {
        auto it = eventMap.find(eventId);
        // Add A New List
        if (it == eventMap.cend())
        {
            eventMap[eventId] = std::list<Object*>();
        }

        auto& eventList = eventMap[eventId];
        for (auto obj : eventList)
        {
            if (obj == this)
            {
                return;
            }
        }

        eventMap[eventId].push_back(this);
    }

    void UnRegister(size_t eventId)
    {
        auto it = eventMap.find(eventId);
        // Add A New List
        if (it == eventMap.cend())
        {
            eventMap[eventId] = std::list<Object*>();
            return;
        }

        auto& eventList = eventMap[eventId];
        for (auto obj : eventList)
        {
            if (obj == this)
            {
                eventList.remove(obj);
                return;
            }
        }
    }

    virtual void OnNotify(const Event& e)
    {

    }

    static void Dispatch(const Event& e)
    {
        auto eventName = e.GetHashID();
        auto& eventList = eventMap[eventName];
        for (auto obj : eventList)
        {
            obj->OnNotify(e);
        }
    }

    static std::map<size_t, std::list<Object*>> eventMap;
};
