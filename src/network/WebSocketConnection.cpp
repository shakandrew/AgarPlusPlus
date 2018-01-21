#ifdef _WIN32
    #include <sdkddkver.h>
#endif

#include <boost/asio/bind_executor.hpp>
#include <boost/beast/core/buffers_to_string.hpp>

#include "WebSocketConnection.h"

namespace websocket = boost::beast::websocket;

WebSocketConnection::WebSocketConnection(ip::tcp::socket tcpSocket)
    : webSocket(std::move(tcpSocket)),
      strand(webSocket.get_executor())
{

}

void WebSocketConnection::run(std::function<void(std::string)> onSignalingMessage)
{
    this->onSignalingMessage = std::move(onSignalingMessage);
    webSocket.async_accept(boost::asio::bind_executor(strand,
                                                      std::bind(&WebSocketConnection::onAccept,
                                                                shared_from_this(),
                                                                std::placeholders::_1)));
}

void WebSocketConnection::send(std::string message)
{
    auto messagePointer = std::make_shared<std::string>(message);
    webSocket.text(true);
    webSocket.async_write(boost::asio::buffer(*messagePointer),
                          boost::asio::bind_executor(strand,
                                                     std::bind(&WebSocketConnection::onSend,
                                                               shared_from_this(),
                                                               std::placeholders::_1,
                                                               std::placeholders::_2,
                                                               messagePointer)));
}

void WebSocketConnection::onAccept(boost::system::error_code errorCode)
{
    if (errorCode) {
        std::cout << "Accept error: " << errorCode.message() << std::endl;
        return;
    }

    receive();
}

void WebSocketConnection::receive()
{
    webSocket.async_read(messageBuffer, boost::asio::bind_executor(strand, std::bind(&WebSocketConnection::onReceive,
                                                                              shared_from_this(),
                                                                              std::placeholders::_1,
                                                                              std::placeholders::_2)));
}

void WebSocketConnection::onReceive(boost::system::error_code errorCode, std::size_t bytesTransferred)
{
    boost::ignore_unused(bytesTransferred);
    if (errorCode == websocket::error::closed) {
        std::cout << "Tried to receive from closed WebSocket." << std::endl;
    }
    else if (errorCode) {
        std::cout << "Receive erorr: " << errorCode.message() << std::endl;
    }
    std::istringstream receivedMessage(boost::beast::buffers_to_string(messageBuffer.data()));
    onSignalingMessage(receivedMessage.str());
}

void WebSocketConnection::onSend(boost::system::error_code errorCode, std::size_t bytesTransferred,
                                 std::shared_ptr<std::string> message)
{
    boost::ignore_unused(bytesTransferred);
    if (errorCode) {
        std::cout << "Send error: " << errorCode.message() << std::endl;
    }
    messageBuffer.consume(messageBuffer.size());

    receive();
}
