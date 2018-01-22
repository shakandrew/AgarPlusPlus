#include <stdexcept>

#include "network/PacketType.h"

std::string packetTypeAsString(PacketType type)
{
    switch (type) {
        case PacketType::JOIN:
            return "JOIN";
        case PacketType::WLCM:
            return "WLCM";
        case PacketType::UPDT:
            return "UPDT";
        case PacketType::INPT:
            return "INPT";
        case PacketType::FRWL:
            return "FRWL";
        default:
            return "packetType";
    }
}

PacketType packetTypeFromString(std::string const &string)
{
    if (string == "JOIN") {
        return PacketType::JOIN;
    } else if (string == "WLCM") {
        return PacketType ::WLCM;
    } else if (string == "UPDT") {
        return PacketType::UPDT;
    } else if (string == "INPT") {
        return PacketType::INPT;
    } else if (string == "FRWL") {
        return PacketType::FRWL;
    } else {
        throw std::invalid_argument("Unsupported packet type.");
    }
}
