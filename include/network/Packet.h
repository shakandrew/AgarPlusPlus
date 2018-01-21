#ifndef AGARPLUSPLUS_PACKET_H
#define AGARPLUSPLUS_PACKET_H

#include <string>

#include "WebRTCConnection.h"

class WebRTCConnection;

class Packet
{
    std::string buffer;

    WebRTCConnection *source;

public:

    Packet();

    Packet(WebRTCConnection *source, std::string buffer);

    Packet& operator=(Packet const &other);

    std::string& getBuffer();

    WebRTCConnection* getSource() const;
};

#endif
