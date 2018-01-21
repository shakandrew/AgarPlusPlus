#ifndef AGARPLUSPLUS_WEBRTCCONFIGURATION_H
#define AGARPLUSPLUS_WEBRTCCONFIGURATION_H

#include <webrtc/api/peerconnectioninterface.h>

class WebRTCConfiguration
{
public:

    WebRTCConfiguration();

    webrtc::PeerConnectionInterface::RTCConfiguration configuration;

    webrtc::DataChannelInit dataChannelConfiguration;

    webrtc::PeerConnectionInterface::IceServer iceServer;
};

#endif
