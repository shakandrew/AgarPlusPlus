#ifdef _WIN32
    #include <sdkddkver.h>
#endif

#include <iostream>

#include "Server.h"

namespace ip = boost::asio::ip;

constexpr int Server::MAX_PACKETS_PER_FRAME;

constexpr int Server::PACKET_QUEUE_CAPACITY;

Server::Server(std::string const &address, unsigned short port)
    : packetQueue(std::make_unique<boost::lockfree::spsc_queue<Packet>>(PACKET_QUEUE_CAPACITY)),
      networkContext(std::make_unique<NetworkContext>()),
      shouldQuit(false),
      timeManager(std::make_unique<TimeManager>()),
      networkManager(std::make_unique<NetworkManager>(ip::tcp::endpoint(ip::address::from_string(address), port),
                                                      networkContext.get(), timeManager.get(), packetQueue.get()))
{

}

void Server::run()
{
    auto processedPackets = 0;

    while(!shouldQuit) {
        timeManager->advance();

        processedPackets = 0;

        while(!packetQueue->empty() && processedPackets < MAX_PACKETS_PER_FRAME) {
            auto &packet = packetQueue->front();
            // TODO: pass server's callback for new players' handling
            networkManager->processPacket(packet);
            if (!packetQueue->pop()) {
                std::cout << "Failed to pop packet from packet queue." << std::endl;
            }
            processedPackets += 1;
        }
        // TODO: pass server's callback for disconnected (timeout) players' handling
        networkManager->disconnectTimedOutPlayers();

        // TODO: player re-spawn

        // TODO: game world update

        // TODO: send outgoing packets
    }
}
