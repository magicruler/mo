#pragma once

#include <string>
#include <functional>

#define MO_OBJECT(NAME) public: virtual std::string GetType() const { return NAME; } static std::string GetTypeStatic() { return NAME; } static size_t GetHashIDStatic(){return std::hash<std::string>{}(NAME);}

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

