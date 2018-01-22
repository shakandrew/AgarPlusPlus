#ifdef _WIN32
    #include <sdkddkver.h>
#endif

#include <iostream>
#include "SignalingManager.h"

SignalingManager::SignalingManager(boost::asio::io_context &ioContext,
                                   const ip::tcp::endpoint endpoint, WebRTCConnectionFactory *factory)
    : ioContext{ioContext},
      acceptor{ioContext},
      socket{ioContext},
      webRTCConnectionFactory{factory}
{
    boost::system::error_code errorCode;
    acceptor.open(endpoint.protocol(), errorCode);
    if (errorCode) {
        std::cout << "Open error: " << errorCode.message() << std::endl;
        return;
    }

    acceptor.bind(endpoint, errorCode);
    if (errorCode) {
        std::cout << "Bind error: " << errorCode.message() << std::endl;
        return;
    }

    acceptor.listen(boost::asio::socket_base::max_listen_connections, errorCode);
    if (errorCode) {
        std::cout << "Listen error: " << errorCode.message() << std::endl;
        return;
    }
}

SignalingManager::~SignalingManager()
{
    ioContext.dispatch([&]()
                       {
                           boost::system::error_code errorCode;
                           acceptor.close(errorCode);
                       }
    );

    webSocketThread->join();
}

void SignalingManager::run()
{
    if (!acceptor.is_open()) {
        std::cout << "Failed to open acceptor socket." << std::endl;
        return;
    }

	webSocketThread = std::make_unique<std::thread>([this]() { ioContext.run(); });

    accept();
}

void SignalingManager::accept()
{
    acceptor.async_accept(socket, std::bind(&SignalingManager::onAccept,
                                            shared_from_this(),
                                            std::placeholders::_1));
}

void SignalingManager::onAccept(boost::system::error_code errorCode)
{
    if (errorCode) {
        std::cout << "Accept error: " << errorCode.message() << std::endl;
        return;
    } else {
        auto webRTCConnection = webRTCConnectionFactory->createNewConnection(std::make_shared<WebSocketConnection>(std::move(socket)));
        webRTCConnection->run();
    }

    accept();
}
