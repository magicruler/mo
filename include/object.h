#pragma once
#include <string>
#include <functional>
#include <map>
#include <vector>
#include <memory>

#define MO_OBJECT(NAME)                                  \
public:                                                  \
    virtual std::string GetType() const { return NAME; } \
    static std::string GetTypeStatic() { return NAME; }  \
    static size_t GetHashIDStatic() { return std::hash<std::string>{}(NAME); }

class Event;

class Object: public std::enable_shared_from_this<Object>
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

    static void SendEvent(std::shared_ptr<Event> event);

    template <typename T>
    void RegisterEventListener()
    {
        size_t hashId = T::GetHashIDStatic();
        RegisterEventListener(hashId);
    }

    void RegisterEventListener(size_t hashId);

    template <typename T>
    void RemoveEventListener()
    {
        size_t hashId = T::GetHashIDStatic();
        RemoveEventListener(hashId);
    }

    void RemoveEventListener(size_t hashId);

    virtual void OnEvent(std::shared_ptr<Event> event)
    {
    }

    static std::map<size_t, std::vector<std::shared_ptr<Object>>> listeners;
};
