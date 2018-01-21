#include "network/Packet.h"

Packet::Packet()
    : source{nullptr},
      buffer{}
{

}

Packet::Packet(WebRTCConnection *source, std::string buffer)
    : source{source},
      buffer{buffer}
{

}

Packet &Packet::operator=(Packet const &other)
{
    source = other.source;
    buffer = other.buffer;
    return *this;
}

WebRTCConnection *Packet::getSource() const
{
    return source;
}

std::string &Packet::getBuffer()
{
    return buffer;
}
