#include <type_traits>

#include "game/ObjectType.h"

ObjectType operator |(ObjectType lhs, ObjectType rhs)
{
    return static_cast<ObjectType>(static_cast<std::underlying_type<ObjectType>::type>(lhs) |
            static_cast<std::underlying_type<ObjectType>::type>(rhs));
}

ObjectType operator &(ObjectType lhs, ObjectType rhs)
{
    return static_cast<ObjectType>(static_cast<std::underlying_type<ObjectType>::type>(lhs) &
            static_cast<std::underlying_type<ObjectType>::type>(rhs));
}