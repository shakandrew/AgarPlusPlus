#ifndef AGARPLUSPLUS_SERVER_H
#define AGARPLUSPLUS_SERVER_H

#include <boost/lockfree/spsc_queue.hpp>

#include <game/Model.h>
#include "network/NetworkManager.h"
#include "time/TimeManager.h"

class Server
{
    static constexpr int FRAMES_PER_SECOND = 1;

    static constexpr double FRAME_DURATION = 1.0 / FRAMES_PER_SECOND;

    static constexpr int MAX_PACKETS_PER_FRAME = 20;

    static constexpr int PACKET_QUEUE_CAPACITY = 100;

    std::unique_ptr<boost::lockfree::spsc_queue<Packet>> packetQueue;

    std::unique_ptr<NetworkContext> networkContext;

    bool shouldQuit;

    std::unique_ptr<TimeManager> timeManager;

    std::unique_ptr<NetworkManager> networkManager;

    std::unique_ptr<Model> gameModel;

public:

    Server(std::string const &address, unsigned short port);

    void run();

};

#endif
