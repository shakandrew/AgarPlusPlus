#ifdef _WIN32
    #include <sdkddkver.h>
#endif

#include <iostream>
#include <thread>

#include "Server.h"

namespace ip = boost::asio::ip;

constexpr int Server::FRAMES_PER_SECOND;

constexpr double Server::FRAME_DURATION;

constexpr int Server::MAX_PACKETS_PER_FRAME;

constexpr int Server::PACKET_QUEUE_CAPACITY;

Server::Server(std::string const &address, unsigned short port)
    : packetQueue(std::make_unique<boost::lockfree::spsc_queue<Packet>>(PACKET_QUEUE_CAPACITY)),
      networkContext(std::make_unique<NetworkContext>()),
      shouldQuit(false),
      timeManager(std::make_unique<TimeManager>()),
      networkManager(std::make_unique<NetworkManager>(ip::tcp::endpoint(ip::address::from_string(address), port),
                                                      networkContext.get(), timeManager.get(), packetQueue.get())),
      gameModel(std::make_unique<Model>())
{
    networkManager->setOnNewPlayerCallback([](PlayerProxy *proxy)
                                           {
                                               auto object = gameModel->createAndAddNewPlayerCell();
                                               auto objectId = networkContext->registerIfAbsentAnGetIdForObject(object);
                                               proxy->setPlayersObjectId(objectId);
                                               // TODO: add created object to every proxy's replicationManager
                                           }
    );

    networkManager->setOnPlayerDisconnectCallback([](PlayerProxy *proxy)
                                                  {
                                                      auto objectId = proxy->getPlayersObjectId();
                                                      auto object = networkContext->getObjectForId(objectId);
                                                      if (!object) {
                                                          std::cout << "No object for such id in network context." << std::endl;
                                                      }
                                                      gameModel->removeObject(object);
                                                      // TODO: remove player's object from every proxy's replicationManager
                                                  }
    );
}

void Server::run()
{
    auto processedPackets = 0;

    while(!shouldQuit) {
        timeManager->advance();
        processedPackets = 0;

        while(!packetQueue->empty() && processedPackets < MAX_PACKETS_PER_FRAME) {
            auto &packet = packetQueue->front();
            networkManager->processPacket(packet);
            if (!packetQueue->pop()) {
                std::cout << "Failed to pop packet from packet queue." << std::endl;
            }
            processedPackets += 1;
        }

        networkManager->disconnectTimedOutPlayers();

        // TODO: player re-spawn

        // TODO: game world update

        networkManager->sendStateUpdatePackets();

        while (timeManager->getTimeSinceLastTick() < FRAME_DURATION) {
            auto sleepTime = static_cast<long>(FRAME_DURATION - timeManager->getTimeSinceLastTick() * 1000);
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
        }
    }
}
