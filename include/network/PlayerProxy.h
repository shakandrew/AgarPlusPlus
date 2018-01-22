#ifndef AGARPLUSPLUS_PLAYERPROXY_H
#define AGARPLUSPLUS_PLAYERPROXY_H

#include "Input.h"
#include "WebRTCConnection.h"
#include "replication/ReplicationManager.h"

class PlayerProxy
{
    WebRTCConnection *connection;

    int id;

    std::size_t indexWithinProxiesContainer;

    std::unique_ptr<Input> lastReceivedInput;
public:
    Input *getLastReceivedInput();

private:

    double lastReceivedPacketTimestamp;

    std::string name;

    int playersObjectId;

    std::unique_ptr<ReplicationManager> replicationManager;

public:

    PlayerProxy(WebRTCConnection *connection, std::string const &name, int id,
                    std::unique_ptr<ReplicationManager> replicationManager);

    void assignPlayersObject(int id);

    WebRTCConnection* getConnection() const;

    std::size_t getIndexWithinProxiesContainer() const;

    double getLastReceivedPacketTimestamp() const;

    std::string const& getName() const;

    int getPlayerId() const;

    int getPlayersObjectId() const;

    ReplicationManager* getReplicationManager();

    bool isLastReceivedInputDirty() const;

    void setIndexWithinProxiesContainer(std::size_t index);

    void setLastReceivedInput(std::unique_ptr<Input> input);

    void setLastReceivedPacketTimestamp(double timestamp);

    void setPlayersObjectId(int id);

};

#endif
