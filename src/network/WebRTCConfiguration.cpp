#include "WebRTCConfiguration.h"

WebRTCConfiguration::WebRTCConfiguration()
{
    iceServer.uri = "stun:stun.l.google.com:19302";
    configuration.servers.push_back(iceServer);
    dataChannelConfiguration.ordered = false;
    dataChannelConfiguration.maxRetransmits = 0;
}
