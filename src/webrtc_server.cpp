#include <boost/asio/signal_set.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/foreach.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <webrtc/api/audio_codecs/builtin_audio_decoder_factory.h>
#include <webrtc/api/audio_codecs/builtin_audio_encoder_factory.h>
#include <webrtc/api/peerconnectioninterface.h>
#include <webrtc/base/physicalsocketserver.h>
#include <webrtc/base/ssladapter.h>

#include <thread>

#include "webrtc_observers.h"

namespace po = boost::program_options;
namespace pt = boost::property_tree;
namespace ws = boost::beast::websocket;

using tcp = boost::asio::ip::tcp;

void onAnswerCreated(webrtc::SessionDescriptionInterface *description);
void onDataChannelCreated(webrtc::DataChannelInterface *channel);
void onDataChannelMessage(const webrtc::DataBuffer &buffer);
void onIceCandidate(const webrtc::IceCandidateInterface *candidate);

void handleConnection(tcp::socket &socket);
void startWebSocketServer(boost::asio::ip::address const &ipAddress, unsigned short const &transportPort);
template<class ConstBufferSequence> std::string to_string(ConstBufferSequence const& buffers);

CreateSessionDescriptionObserver createSessionDescriptionObserver(onAnswerCreated);
DataChannelObserver dataChannelObserver(onDataChannelMessage);
PeerConnectionObserver peerConnectionObserver(onDataChannelCreated, onIceCandidate);
SetSessionDescriptionObserver setSessionDescriptionObserver;

rtc::scoped_refptr<webrtc::DataChannelInterface> dataChannel;
static volatile bool keepRunning = true;
static boost::lockfree::spsc_queue<std::string> messageQueue{10};
rtc::scoped_refptr<webrtc::PeerConnectionInterface> peerConnection;
rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> peerConnectionFactory;
ws::stream<tcp::socket>* webSocket;

class SignalingRunnable : public rtc::Runnable {
public:

    void Run(rtc::Thread* thread) override {
        peerConnectionFactory = webrtc::CreatePeerConnectionFactory();
        if (peerConnectionFactory.get() == nullptr) {
            std::cout << "Error: Could not create CreatePeerConnectionFactory." << std::endl;
            return;
        }
        thread->Run();
    }
};

int main(int argc, char const* argv[])
{

    po::options_description description("Program options");
    description.add_options()
            ("address,a", po::value<std::string>()->default_value("127.0.0.1"), "server IP address")
            ("help,h", "show this message")
            ("port,p", po::value<unsigned short>()->default_value(32168), "server transport layer port")
            ;
    po::variables_map variablesMap;
    po::store(po::parse_command_line(argc, argv, description), variablesMap);
    po::notify(variablesMap);

    if (variablesMap.count("help"))
    {
        std::cout << description << std::endl;
        return 0;
    }
    boost::asio::ip::address ipAddress;
    if (variablesMap.count("address") == 1) {
        ipAddress = boost::asio::ip::address::from_string(
                variablesMap["address"].as<std::string>()
        );
    }
    unsigned short transportPort;
    if (variablesMap.count("port") == 1) {
        transportPort = variablesMap["port"].as<unsigned short>();
    }

    std::thread webSocketThread{std::bind(
            &startWebSocketServer,
            ipAddress,
            transportPort)};

    rtc::InitializeSSL();
    rtc::InitRandom(rtc::Time());
    std::unique_ptr<rtc::Thread> signalingThread = std::make_unique<rtc::Thread>();
    signalingThread->SetName("signalingThread", nullptr);
    SignalingRunnable signalingRunnable;
    if (!signalingThread->Start(&signalingRunnable))
    {
        return EXIT_FAILURE;
    }

    while(keepRunning)
    {
        if (!messageQueue.empty())
        {
            std::cout << messageQueue.front() << std::endl;
            messageQueue.pop();
        }
    }

    webSocketThread.join();
    peerConnectionFactory.release();
    rtc::CleanupSSL();
    return 0;
}

void onAnswerCreated(webrtc::SessionDescriptionInterface *description)
{
    peerConnection->SetLocalDescription(&setSessionDescriptionObserver, description);
    std::string sdpAnswerData;
    description->ToString(&sdpAnswerData);
    pt::ptree sdpAnswerDocument;
    sdpAnswerDocument.put("type", "answer");
    pt::ptree sdpAnswerPayloadDocument;
    sdpAnswerPayloadDocument.put("type", "answer");
    sdpAnswerPayloadDocument.put("sdp", sdpAnswerData.c_str());
    sdpAnswerDocument.add_child("payload", sdpAnswerPayloadDocument);
    std::ostringstream sdpAnswerStream;
    pt::write_json(sdpAnswerStream, sdpAnswerDocument);
    webSocket->text(true);
    webSocket->write(boost::asio::buffer(sdpAnswerStream.str()));
}

void onDataChannelCreated(webrtc::DataChannelInterface *channel)
{
    dataChannel = channel;
    dataChannel->RegisterObserver(&dataChannelObserver);
}

void onDataChannelMessage(const webrtc::DataBuffer &buffer)
{
    std::string receivedData(buffer.data.data<char>(), buffer.data.size());
    messageQueue.push(static_cast<std::string>(receivedData));
    webrtc::DataBuffer answerData(rtc::CopyOnWriteBuffer(receivedData.c_str(), receivedData.length()), false);
    dataChannel->Send(answerData);
}

void onIceCandidate(const webrtc::IceCandidateInterface *candidate)
{
   std::string iceCandidateData;
   candidate->ToString(&iceCandidateData);
   pt::ptree iceCandidateDocument;
   iceCandidateDocument.put("type", "candidate");
   pt::ptree iceCandidatePayloadDocument;
   iceCandidatePayloadDocument.put("candidate", iceCandidateData.c_str());
   iceCandidatePayloadDocument.put("sdpMid", candidate->sdp_mid().c_str());
   iceCandidatePayloadDocument.put("sdpMLineIndex", candidate->sdp_mline_index());
   iceCandidateDocument.add_child("payload", iceCandidatePayloadDocument);
   std::ostringstream iceCandidateStream;
   pt::write_json(iceCandidateStream, iceCandidateDocument);
   webSocket->text(true);
   webSocket->write(boost::asio::buffer(iceCandidateStream.str()));
}

void handleConnection(tcp::socket &socket)
{
   try
   {
      std::cout << socket.remote_endpoint().address().to_string() << std::endl
                << socket.remote_endpoint().port() << std::endl;
      ws::stream<tcp::socket> webSocket{std::move(socket)};
      ::webSocket = &webSocket;
      webSocket.accept();
      for(;;)
      {
         boost::beast::multi_buffer receivedData;
         webSocket.read(receivedData);
         std::istringstream receivedDataStream{to_string(receivedData.data())};
         pt::ptree messageDocument;
         pt::read_json(receivedDataStream, messageDocument);
         auto messageType = messageDocument.get<std::string>("type");
         if (messageType == "offer")
         {
            std::string sdp = messageDocument.get<std::string>("payload.sdp");
            webrtc::PeerConnectionInterface::RTCConfiguration configuration;
            webrtc::PeerConnectionInterface::IceServer iceServer;
            iceServer.uri = "stun:stun.l.google.com:19302";
            configuration.servers.push_back(iceServer);
            peerConnection = peerConnectionFactory->CreatePeerConnection(configuration, nullptr, nullptr, &peerConnectionObserver);
            webrtc::DataChannelInit dataChannelConfig;
            dataChannelConfig.ordered = false;
            dataChannelConfig.maxRetransmits = 0;
            dataChannel = peerConnection->CreateDataChannel("dc", &dataChannelConfig);
            dataChannel->RegisterObserver(&dataChannelObserver);
            webrtc::SdpParseError error;
            webrtc::SessionDescriptionInterface* sessionDescription(webrtc::CreateSessionDescription("offer", sdp, &error));
            peerConnection->SetRemoteDescription(&setSessionDescriptionObserver, sessionDescription);
            peerConnection->CreateAnswer(&createSessionDescriptionObserver, nullptr);
         }
         else if (messageType == "candidate")
         {
            std::string candidate = messageDocument.get<std::string>("payload.candidate");
            int sdpMLineIndex = messageDocument.get<int>("payload.sdpMLineIndex");
            std::string sdpMid = messageDocument.get<std::string>("payload.sdpMid");
            webrtc::SdpParseError error;
            auto iceCandidate = webrtc::CreateIceCandidate(sdpMid, sdpMLineIndex, candidate, &error);
            peerConnection->AddIceCandidate(iceCandidate);
         }
         else
         {
            std::cout << "Unknown message type" << std::endl;
         }
      }
   }
   catch(boost::system::system_error const& se)
   {
      if(se.code() != ws::error::closed)
      {
         std::cerr << "Error: " << se.code().message() << std::endl;
      }
   }
   catch(std::exception const& e)
   {
      std::cerr << "Error: " << e.what() << std::endl;
   }
}

void startWebSocketServer(boost::asio::ip::address const &ipAddress, unsigned short const &transportPort)
{
    try
    {
        boost::asio::io_service ioc{1};

        tcp::acceptor acceptor{ioc, {ipAddress, transportPort}};

        tcp::socket socket{ioc};

        while (keepRunning)
        {
            acceptor.accept(socket);

            std::thread{std::bind(
                    &handleConnection,
                    std::move(socket))}.detach();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

template<class ConstBufferSequence> std::string to_string(ConstBufferSequence const& buffers)
{
    std::string s;
    s.reserve(boost::asio::buffer_size(buffers));
    for(boost::asio::const_buffer b : buffers)
        s.append(boost::asio::buffer_cast<char const*>(b),
                 boost::asio::buffer_size(b));
    return s;
}

/*void handler(const boost::system::error_code& error, int signal_number)
{
    if (!error)
    {
        if (signal_number == SIGINT)
        {

        }
    }
}*/