#ifndef AGARPLUSPLUS_REPLICATIONMANAGER_H
#define AGARPLUSPLUS_REPLICATIONMANAGER_H

#include <unordered_map>

#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>

#include "network/NetworkContext.h"
#include "ReplicationCommand.h"

class ReplicationManager
{
    NetworkContext *networkContext;

    std::unordered_map<int, std::unique_ptr<ReplicationCommand>> networkIdToReplicationCommand;

    std::vector<int> pendingRemovalNetworkIds;

public:

    explicit ReplicationManager(NetworkContext *networkContext);

    ReplicationManager(ReplicationManager const &) = delete;

    void destroyObject(int networkId);

    void instantiateObject(int networkId);

    void serialize(PacketDocument document);

    void stopReplicatingObject(int networkId);

    void updateObject(int networkId);
};

#endif
