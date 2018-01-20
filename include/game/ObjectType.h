#ifndef AGAR_OBJECTTYPE_H
#define AGAR_OBJECTTYPE_H

#include <cstdint>

enum class ObjectType : uint32_t
{
    NONE = 0,
    PLAYER = (1 << 0),
    FOOD = (1 << 1),
    RANGE = (1 << 2)
};

ObjectType operator |(ObjectType lhs, ObjectType rhs);
ObjectType operator &(ObjectType lhs, ObjectType rhs);

#endif
