#ifndef AGARPLUSPLUS_PACKETTYPE_H
#define AGARPLUSPLUS_PACKETTYPE_H

#include <string>

enum class PacketType
{
    JOIN,
    WLCM,
    UPDT,
    INPT,
    FRWL,
    MAX_VALUE
};

std::string packetTypeAsString(PacketType type);

PacketType packetTypeFromString(std::string const &string);

#endif
