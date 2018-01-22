#ifndef AGARPLUSPLUS_WEBSOCKETCONNECTION_H
#define AGARPLUSPLUS_WEBSOCKETCONNECTION_H

#include <memory>

#include <boost/asio/strand.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/multi_buffer.hpp>
#include <boost/beast/websocket/stream.hpp>

namespace ip = boost::asio::ip;

class WebSocketConnection : public std::enable_shared_from_this<WebSocketConnection>
{
    boost::beast::multi_buffer messageBuffer;

    std::function<void(std::string)> onSignalingMessage;

    boost::beast::websocket::stream<ip::tcp::socket> webSocket;

    boost::asio::strand<boost::asio::io_context::executor_type> strand;

public:

    explicit WebSocketConnection(ip::tcp::socket tcpSocket);

    void close();

    void run(std::function<void(std::string)> onSignalingMessage);

    void send(std::string message);

private:

    void onAccept(boost::system::error_code errorCode);

    void receive();

    void onReceive(boost::system::error_code errorCode, std::size_t bytesTransferred);

    void onSend(boost::system::error_code errorCode, std::size_t bytesTransferred, std::shared_ptr<std::string> message);
};

#endif
