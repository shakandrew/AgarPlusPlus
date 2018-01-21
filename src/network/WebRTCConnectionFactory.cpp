#include "WebRTCConnectionFactory.h"

WebRTCConnectionFactory::WebRTCConnectionFactory(WebRTCConfiguration *configuration,
                                                 boost::lockfree::spsc_queue<Packet> *packetQueue)
    : configuration(configuration),
      packetQueue(packetQueue)
{

}

std::shared_ptr<WebRTCConnection>
WebRTCConnectionFactory::createNewConnection(std::shared_ptr<WebSocketConnection> signalingConnection)
{
    return std::make_shared<WebRTCConnection>(configuration, peerConnectionFactory, signalingConnection, packetQueue);
}

void WebRTCConnectionFactory::run()
{
    peerConnectionFactory = webrtc::CreateModularPeerConnectionFactory(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    if (peerConnectionFactory.get() == nullptr) {
        std::cout << "Error: Could not create CreatePeerConnectionFactory." << std::endl;
        return;
    }
    rtc::Thread *signalingThread = rtc::Thread::Current();
    signalingThread->Run();
}
