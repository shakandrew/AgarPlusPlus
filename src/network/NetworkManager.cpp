#ifdef _WIN32
    #include <sdkddkver.h>
#endif

#include <iostream>
#include <thread>

#include <boost/asio/ip/tcp.hpp>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <webrtc/rtc_base/ssladapter.h>

#include "network/NetworkManager.h"

using namespace rapidjson;

namespace ip = boost::asio::ip;

constexpr int NetworkManager::SIGNALING_THREADS;

NetworkManager::NetworkManager(const ip::tcp::endpoint endpoint, NetworkContext *networkContext,
                               TimeManager *timeManager,
                               boost::lockfree::spsc_queue<Packet> *packetQueue)
    : ioContext{SIGNALING_THREADS},
      nextPlayerId{1},
      configuration{std::make_unique<WebRTCConfiguration>()},
      networkContext{networkContext},
      packetQueue{packetQueue},
      playerDisconnectTimeout{15},
      webRTCConnectionFactory{std::make_shared<WebRTCConnectionFactory>(configuration.get(), packetQueue)},
      signalingManager{std::make_shared<SignalingManager>(ioContext, endpoint, webRTCConnectionFactory.get())},
      timeManager{timeManager}
{
    signalingManager->run();
    std::thread webSocketThread([this]() { ioContext.run(); });
    webSocketThread.detach();

    rtc::InitRandom(rtc::Time32());
    rtc::InitializeSSL();
    std::thread signalingThread{[this]() { webRTCConnectionFactory->run(); }};
    signalingThread.detach();
}

void NetworkManager::processPacket(Packet &packet)
{
    auto packetSourcePair = connectionToPlayerProxy.find(packet.getSource());
    if (packetSourcePair == connectionToPlayerProxy.end()) {
        processPacketFromNewPlayer(packet);
    } else {
        auto packetSourceProxy = packetSourcePair->second;
        processPacketFromExistingPlayer(packetSourceProxy, packet);
    }
}

void NetworkManager::processPacketFromNewPlayer(Packet &packet)
{
    // TODO: parse into PacketDocument object
    rapidjson::Document message;
    message.Parse(packet.getBuffer().c_str());

    if (!message.IsObject()) {
        std::cout << "Malformed message, ignoring..." << std::endl;
        return;
    }

    auto packetTypeMemberName = packetTypeAsString(PacketType::MAX_VALUE);
    if (!message.HasMember(packetTypeMemberName.c_str())) {
        std::cout << "packetType is missing, ignoring..." << std::endl;
        return;
    }

    auto &packetTypeValue = message[packetTypeMemberName.c_str()];
    if (!packetTypeValue.IsString()) {
        std::cout << "packetType is not a string, ignoring..." << std::endl;
        return;
    }

    std::string packetType = packetTypeValue.GetString();
    if (packetType == packetTypeAsString(PacketType::JOIN)) {
        if (!message.HasMember("playerName")) {
            std::cout << "JOIN packet is missing player name, ignoring..." << std::endl;
            return;
        }

        auto &playerNameValue = message["playerName"];
        if (!playerNameValue.IsString()) {
            std::cout << "playerName is not a string, ignoring..." << std::endl;
        }

        std::string playerName = playerNameValue.GetString();
        auto playerProxy = registerPlayer(packet.getSource(), playerName);
        if (onNewPlayer) {
            onNewPlayer(playerProxy);
        }
        playerProxy->setLastReceivedPacketTimestamp(timeManager->getTime());

        sendWelcomePacket(playerProxy);

        auto playerReplicationManager = playerProxy->getReplicationManager();
        // TODO: limit this to player's range
        for (auto const &pair : networkContext->getAllIdToObjectPairs()) {
            playerReplicationManager->instantiateObject(pair.first);
        }
    }
}

int NetworkManager::getNextPlayerId()
{
    auto id = nextPlayerId;
    nextPlayerId += 1;
    return id;
}

PlayerProxy* NetworkManager::registerPlayer(WebRTCConnection *connection, std::string const &name)
{
    auto id = getNextPlayerId();
    auto replicationManager = std::make_unique<ReplicationManager>(networkContext);
    auto proxy = std::make_unique<PlayerProxy>(connection, name, id, std::move(replicationManager));
    auto proxyPointer = proxy.get();
    proxyPointer->setIndexWithinProxiesContainer(playersProxies.size());
    playersProxies.push_back(std::move(proxy));
    connectionToPlayerProxy[connection] = proxyPointer;
    playerIdToPlayerProxy[id] = proxyPointer;
    return proxyPointer;
}

void NetworkManager::sendWelcomePacket(PlayerProxy *proxy)
{
    // TODO: build PacketDocument object
    Document message;
    message.SetObject();
    auto &allocator = message.GetAllocator();

    message.AddMember("packetType", Value().SetString(packetTypeAsString(PacketType::WLCM).c_str(), allocator), allocator);
    message.AddMember("playerId", Value().SetInt(proxy->getPlayerId()), allocator);

    std::cout << "Welcome message sent to: " << proxy->getPlayerId() << std::endl;

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    message.Accept(writer);

    sendPacket(buffer.GetString(), proxy->getConnection());
}

void NetworkManager::sendPacket(std::string const &buffer, WebRTCConnection *connection)
{
    connection->send(buffer);
}

void NetworkManager::processPacketFromExistingPlayer(PlayerProxy *proxy, Packet &packet)
{
    proxy->setLastReceivedPacketTimestamp(timeManager->getTime());

    // TODO: parse into PacketDocument object
    rapidjson::Document message;
    message.Parse(packet.getBuffer().c_str());

    if (!message.IsObject()) {
        std::cout << "Malformed message, ignoring..." << std::endl;
        return;
    }

    if (!message.HasMember("packetType")) {
        std::cout << "packetType is missing, ignoring..." << std::endl;
        return;
    }

    auto &packetTypeValue = message["packetType"];
    if (!packetTypeValue.IsString()) {
        std::cout << "packetType is not a string, ignoring..." << std::endl;
        return;
    }

    std::string packetType = packetTypeValue.GetString();
    if (packetType == packetTypeAsString(PacketType::JOIN)) {
        if (!message.HasMember("playerName")) {
            std::cout << "JOIN packet is missing player name, ignoring..." << std::endl;
            return;
        }

        auto &playerNameValue = message["playerName"];
        if (!playerNameValue.IsString()) {
            std::cout << "playerName is not a string, ignoring..." << std::endl;
            return;
        }

        std::string playerName = playerNameValue.GetString();
        if (proxy->getName() != playerName) {
            std::cout << "Player " << proxy->getPlayerId() << " has sent JOIN message again with different name." << std::endl;
        }

        sendWelcomePacket(proxy);
    } else if (packetType == packetTypeAsString(PacketType::INPT)) {
        if (!message.HasMember("angle")) {
            std::cout << "INPT packet is missing angle value, ignoring..." << std::endl;
            return;
        }
        auto &angleValue = message["angle"];
        if (!angleValue.IsInt()) {
            std::cout << "angle is not an integer, ignoring..." << std::endl;
            return;
        }
        int angle = angleValue.GetInt();

        if (!message.HasMember("speed")) {
            std::cout << "INPT packet is missing speed value, ignoring..." << std::endl;
            return;
        }
        auto &speedValue = message["speed"];
        if (!speedValue.IsInt()) {
            std::cout << "speed is not an integer, ignoring..." << std::endl;
            return;
        }
        auto speed = speedValue.GetInt();

        std::cout << "Player " << proxy->getPlayerId() << " submitted input: angle = " << angle << ", speed = " << speed << "." << std::endl;

        proxy->setLastReceivedInput(std::move(std::make_unique<Input>(angle, speed)));

    } else {
        std::cout << "Player" << proxy->getPlayerId() << " has sent message of unknown type." << std::endl;
    }
}

void NetworkManager::disconnectTimedOutPlayers()
{
    std::vector<PlayerProxy *> proxiesOfPlayersToDisconnect;
    double oldestAllowedLastReceivedPacketTimestamp = timeManager->getTime() - playerDisconnectTimeout;
    for (auto const &pair : connectionToPlayerProxy) {
        auto proxy = pair.second;
        if (proxy->getLastReceivedPacketTimestamp() < oldestAllowedLastReceivedPacketTimestamp) {
            proxiesOfPlayersToDisconnect.push_back(proxy);
        }
    }
    for (auto &proxy : proxiesOfPlayersToDisconnect) {
        disconnectPlayer(proxy);
    }
}

void NetworkManager::disconnectPlayer(PlayerProxy *proxy)
{
    std::cout << "Disconnecting id " << proxy->getPlayerId() << std::endl;
    if (onPlayerDisconnect) {
        onPlayerDisconnect(proxy);
    }
    playerIdToPlayerProxy.erase(proxy->getPlayerId());
    connectionToPlayerProxy.erase(proxy->getConnection());
    auto lastProxy = std::move(playersProxies.back());
    playersProxies.pop_back();
    if (proxy != lastProxy.get()) {
        auto proxyIndex = proxy->getIndexWithinProxiesContainer();
        lastProxy->setIndexWithinProxiesContainer(static_cast<std::size_t>(proxyIndex));
        playersProxies[proxyIndex] = std::move(lastProxy);
    }
}

void NetworkManager::setOnNewPlayerCallback(std::function<void(PlayerProxy *)> onNewPlayer)
{
    this->onNewPlayer = std::move(onNewPlayer);
}

void NetworkManager::setOnPlayerDisconnectCallback(std::function<void(PlayerProxy *)> onPlayerDisconnect)
{
    this->onPlayerDisconnect = std::move(onPlayerDisconnect);
}
