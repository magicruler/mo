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
    virtual size_t GetHashID() { return std::hash<std::string>{}(NAME); }\
    static size_t GetHashIDStatic() { return std::hash<std::string>{}(NAME); }

class Event;

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
};
