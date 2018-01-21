#include <network/PacketDocument.h>
#include "replication/ReplicationManager.h"

ReplicationManager::ReplicationManager(NetworkContext *networkContext)
    : networkContext{networkContext}
{

}

void ReplicationManager::destroyObject(int networkId)
{
    networkIdToReplicationCommand[networkId]->convertToDestroy();
}

void ReplicationManager::instantiateObject(int networkId)
{
    networkIdToReplicationCommand[networkId] = std::move(std::make_unique<ReplicationCommand>(ReplicationActionType::INST));
}

void ReplicationManager::serialize(PacketDocument document)
{
    for (auto &idToCommand : networkIdToReplicationCommand) {
        auto command = idToCommand.second.get();
        if (command->getIsDirty()) {
            auto id = idToCommand.first;
            document.serializeNetworkId(id);
            auto action = command->getAction();
            document.serializeActionType(action);
            if (action == ReplicationActionType::INST) {
                auto object = networkContext->getObjectForId(id);
                document.serializeObjectType(object->getType());
                object->serialize(document);
                command->convertToUpdate();
            } else if (action == ReplicationActionType::DSTR) {
                pendingRemovalNetworkIds.push_back(id);
            } else if (action == ReplicationActionType::UPDT) {
                auto object = networkContext->getObjectForId(id);
                document.serializeObjectType(object->getType());
                object->serialize(document);
            }
            command->setAsClean();
        }
    }

    if (!pendingRemovalNetworkIds.empty()) {
        for (auto id : pendingRemovalNetworkIds) {
            stopReplicatingObject(id);
        }
        pendingRemovalNetworkIds.clear();
    }
}

void ReplicationManager::stopReplicatingObject(int networkId)
{
    networkIdToReplicationCommand.erase(networkId);
}

void ReplicationManager::updateObject(int networkId)
{
    networkIdToReplicationCommand[networkId]->setAsDirty();
}
