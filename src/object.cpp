#include "object.h"

std::map<size_t, std::list<Object*>> Object::eventMap = std::map<size_t, std::list<Object*>>();