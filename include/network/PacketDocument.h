#ifndef AGARPLUSPLUS_PACKETDOCUMENT_H
#define AGARPLUSPLUS_PACKETDOCUMENT_H

#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>

#include "game/GameObject.h"
#include "game/ObjectType.h"
#include "network/PacketType.h"
#include "replication/ReplicationActionType.h"

class PacketDocument
{
    rapidjson::Document message;

public:

    explicit PacketDocument(PacketType type);

    void serializeNetworkId(int id);

    void serializeActionType(ReplicationActionType type);

    void serializeObjectType(ObjectType type);

    void serializeHorizontalPosition(int x);

    void serializeVerticalPosition(int y);

    void serializeRadius(int r);

private:

    void buildUpdatePacket();
};

#endif
