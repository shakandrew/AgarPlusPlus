#ifndef AGARPLUSPLUS_WEBRTCCONNECTIONFACTORY_H
#define AGARPLUSPLUS_WEBRTCCONNECTIONFACTORY_H

#include <boost/lockfree/spsc_queue.hpp>

#include <webrtc/api/peerconnectioninterface.h>
#include <webrtc/media/engine/webrtcmediaengine.h>

#include "Packet.h"
#include "WebRTCConfiguration.h"

class WebRTCConnectionFactory
{
private:

    WebRTCConfiguration *configuration;

    boost::lockfree::spsc_queue<Packet> *packetQueue;

    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> peerConnectionFactory;

public:

    WebRTCConnectionFactory(WebRTCConfiguration *configuration, boost::lockfree::spsc_queue<Packet> *packetQueue);

    std::shared_ptr<WebRTCConnection> createNewConnection(std::shared_ptr<WebSocketConnection> signalingConnection);

    void run();
};

#endif
