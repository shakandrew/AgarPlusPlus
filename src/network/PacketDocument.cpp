#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "network/PacketDocument.h"

using namespace rapidjson;

PacketDocument::PacketDocument(PacketType type)
{
    message.SetObject();
    auto &allocator = message.GetAllocator();
    auto typeAsString = packetTypeAsString(type);
    message.AddMember("packetType", Value().SetString(typeAsString.c_str(), allocator), allocator);
    switch (type) {
        case PacketType::UPDT:
            buildUpdatePacket();
            break;
        default:
            throw std::invalid_argument("Unsupported packet type.");
    }
}

void PacketDocument::buildUpdatePacket()
{
    auto &allocator = message.GetAllocator();
    message.AddMember("networkId", Value(kArrayType), allocator);
    message.AddMember("actionType", Value(kArrayType), allocator);
    message.AddMember("objectType", Value(kArrayType), allocator);
    message.AddMember("x", Value(kArrayType), allocator);
    message.AddMember("y", Value(kArrayType), allocator);
    message.AddMember("r", Value(kArrayType), allocator);
}

void PacketDocument::serializeNetworkId(int id)
{
    auto &allocator = message.GetAllocator();
    message["networkId"].PushBack(id, allocator);
}

void PacketDocument::serializeActionType(ReplicationActionType type)
{
    auto &allocator = message.GetAllocator();
    auto typeAsString = replicationActionTypeAsString(type);
    message["actionType"].PushBack(Value().SetString(typeAsString.c_str(), allocator), allocator);
}

void PacketDocument::serializeObjectType(ObjectType type)
{
    auto &allocator = message.GetAllocator();
    auto typeAsString = objectTypeAsString(type);
    message["objectType"].PushBack(Value().SetString(typeAsString.c_str(), allocator), allocator);
}

void PacketDocument::serializeHorizontalPosition(int x)
{
    auto &allocator = message.GetAllocator();
    message["x"].PushBack(x, allocator);
}

void PacketDocument::serializeVerticalPosition(int y)
{
    auto &allocator = message.GetAllocator();
    message["y"].PushBack(y, allocator);
}

void PacketDocument::serializeRadius(int r)
{
    auto &allocator = message.GetAllocator();
    message["r"].PushBack(r, allocator);
}

std::string PacketDocument::toString()
{
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    message.Accept(writer);

    return buffer.GetString();
}
