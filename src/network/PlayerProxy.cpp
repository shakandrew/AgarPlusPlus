#include "network/PlayerProxy.h"

PlayerProxy::PlayerProxy(WebRTCConnection *connection, std::string const &name, int id,
                         std::unique_ptr<ReplicationManager> replicationManager)
    : connection{connection},
      id{id},
      name{name},
      replicationManager{std::move(replicationManager)}
{

}

ReplicationManager *PlayerProxy::getReplicationManager()
{
    return replicationManager.get();
}

WebRTCConnection *PlayerProxy::getConnection() const
{
    return connection;
}

std::size_t PlayerProxy::getIndexWithinProxiesContainer() const
{
    return indexWithinProxiesContainer;
}

std::string const &PlayerProxy::getName() const
{
    return name;
}

int PlayerProxy::getPlayerId() const
{
    return id;
}

void PlayerProxy::setIndexWithinProxiesContainer(std::size_t index)
{
    indexWithinProxiesContainer = index;
}

void PlayerProxy::setLastReceivedInput(std::unique_ptr<Input> input)
{
    lastReceivedInput = std::move(input);
}

void PlayerProxy::setLastReceivedPacketTimestamp(double timestamp)
{
    lastReceivedPacketTimestamp = timestamp;
}

double PlayerProxy::getLastReceivedPacketTimestamp() const
{
    return lastReceivedPacketTimestamp;
}
