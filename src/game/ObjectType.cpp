#include <stdexcept>
#include <type_traits>

#include "game/ObjectType.h"

std::string objectTypeAsString(ObjectType type)
{
    switch (type) {
        case ObjectType::PLAYER:
            return "PLYR";
        case ObjectType::FOOD:
            return "FOOD";
        case ObjectType::BOOST:
            return "BOST";
        case ObjectType::RANGE:
            return "RANG";
        default:
            throw std::invalid_argument("Unsupported object type.");
            break;
    }
}

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