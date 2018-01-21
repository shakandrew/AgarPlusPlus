#ifndef AGARPLUSPLUS_NETWORKMANAGER_H
#define AGARPLUSPLUS_NETWORKMANAGER_H

#include <boost/asio/io_context.hpp>
#include <boost/lockfree/spsc_queue.hpp>

#include "NetworkContext.h"
#include "Packet.h"
#include "PlayerProxy.h"
#include "SignalingManager.h"
#include "WebRTCConnection.h"
#include "time/TimeManager.h"
#include "WebRTCConnectionFactory.h"

class NetworkManager
{
    static constexpr int SIGNALING_THREADS = 1;

    boost::asio::io_context ioContext;

    int nextPlayerId;

    std::unique_ptr<WebRTCConfiguration> configuration;

    NetworkContext *networkContext;

    boost::lockfree::spsc_queue<Packet> *packetQueue;

    double playerDisconnectTimeout;

    // TODO: change to unique_ptr
    std::shared_ptr<WebRTCConnectionFactory> webRTCConnectionFactory;

    std::shared_ptr<SignalingManager> signalingManager;

    TimeManager *timeManager;

    std::vector<std::unique_ptr<PlayerProxy>> playersProxies;

    std::unordered_map<WebRTCConnection *, PlayerProxy *> connectionToPlayerProxy;

    std::unordered_map<int, PlayerProxy *> playerIdToPlayerProxy;

public:

    NetworkManager(const ip::tcp::endpoint endpoint, NetworkContext *networkContext, TimeManager *timeManager,
                   boost::lockfree::spsc_queue<Packet> *packetQueue);

    void processPacket(Packet &packet);

    void disconnectTimedOutPlayers();

private:

    void processPacketFromNewPlayer(Packet &packet);

    void processPacketFromExistingPlayer(PlayerProxy *proxy, Packet &packet);

    void sendWelcomePacket(PlayerProxy *proxy);

    void sendPacket(std::string const &buffer, WebRTCConnection *connection);

    int getNextPlayerId();

    PlayerProxy* registerPlayer(WebRTCConnection *connection, std::string const &name);

    void disconnectPlayer(PlayerProxy *proxy);
};

#endif