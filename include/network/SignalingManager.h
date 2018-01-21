#ifndef AGARPLUSPLUS_SIGNALINGMANAGER_H
#define AGARPLUSPLUS_SIGNALINGMANAGER_H

#include <memory>

#include <boost/asio/ip/tcp.hpp>
#include "WebRTCConnectionFactory.h"

namespace ip = boost::asio::ip;

class SignalingManager : public std::enable_shared_from_this<SignalingManager>
{
    ip::tcp::acceptor acceptor;

    ip::tcp::socket socket;

    WebRTCConnectionFactory *webRTCConnectionFactory;

public:
    SignalingManager(boost::asio::io_context &ioContext, const ip::tcp::endpoint endpoint,
                     WebRTCConnectionFactory *factory);

    void run();

private:

    void accept();

    void onAccept(boost::system::error_code errorCode);
};

#endif
