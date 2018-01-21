#ifndef AGARPLUSPLUS_OBJECTTYPE_H
#define AGARPLUSPLUS_OBJECTTYPE_H

#include <cstdint>
#include <string>

enum class ObjectType : uint32_t
{
    NONE = 0,
    PLAYER = (1 << 0),
    FOOD = (1 << 1),
    BOOST = (1 << 2),
    RANGE = (1 << 3)
};

std::string objectTypeAsString(ObjectType type);
ObjectType operator |(ObjectType lhs, ObjectType rhs);
ObjectType operator &(ObjectType lhs, ObjectType rhs);

#endif
