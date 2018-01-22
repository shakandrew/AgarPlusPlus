#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "network/WebRTCConnection.h"

namespace pt = boost::property_tree;

void WebRTCConnection::send(std::string const &message)
{
    dataChannel->Send(webrtc::DataBuffer(message));
}

WebRTCConnection::WebRTCConnection(WebRTCConfiguration *configuration,
                                   rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> peerConnectionFactory,
                                   std::shared_ptr<WebSocketConnection> signalingConnection,
                                   boost::lockfree::spsc_queue<Packet> *packetQueue)
        : configuration(configuration),
          peerConnectionFactory(peerConnectionFactory),
          signalingConnection(std::move(signalingConnection)),
          packetQueue(packetQueue)
{

}

void WebRTCConnection::close()
{
    dataChannel->UnregisterObserver();
    dataChannel->Close();
    peerConnection->Close();
    signalingConnection->close();
}

void WebRTCConnection::handleSignaling(std::string message)
{
    std::stringstream messageStream;
    messageStream << message << std::flush;
    pt::ptree messageDocument;
    pt::read_json(messageStream, messageDocument);

    auto messageType = messageDocument.get<std::string>("type");
    if (messageType == "offer") {
        peerConnection = peerConnectionFactory->CreatePeerConnection(configuration->configuration, nullptr, nullptr, this);
        dataChannel = peerConnection->CreateDataChannel("dc", &(configuration->dataChannelConfiguration));
        dataChannel->RegisterObserver(this);
        auto sdp = messageDocument.get<std::string>("payload.sdp");
        webrtc::SdpParseError error;
        webrtc::SessionDescriptionInterface* sessionDescription(webrtc::CreateSessionDescription("offer", sdp, &error));
        peerConnection->SetRemoteDescription(this, sessionDescription);
        peerConnection->CreateAnswer(this, nullptr);
    }
    else if (messageType == "candidate") {
        auto candidate = messageDocument.get<std::string>("payload.candidate");
        auto sdpMLineIndex = messageDocument.get<int>("payload.sdpMLineIndex");
        auto sdpMid = messageDocument.get<std::string>("payload.sdpMid");
        webrtc::SdpParseError error;
        auto iceCandidate = webrtc::CreateIceCandidate(sdpMid, sdpMLineIndex, candidate, &error);
        peerConnection->AddIceCandidate(iceCandidate);
    }
    else {
        std::cout  << "Unknown message type." << std::endl;
    }
}

void WebRTCConnection::run()
{
    signalingConnection->run(std::bind(&WebRTCConnection::handleSignaling, shared_from_this(), std::placeholders::_1));
}

int WebRTCConnection::AddRef() const
{
    return 0;
}

int WebRTCConnection::Release() const
{
    return 0;
}

void WebRTCConnection::OnFailure(const std::string &)
{

}

void WebRTCConnection::OnSuccess(webrtc::SessionDescriptionInterface *description)
{
    peerConnection->SetLocalDescription(this, description);
    std::string sdpAnswerString;
    description->ToString(&sdpAnswerString);
    pt::ptree sdpAnswerDocument;
    sdpAnswerDocument.put("type", "answer");
    pt::ptree sdpAnswerPayloadDocument;
    sdpAnswerPayloadDocument.put("type", "answer");
    sdpAnswerPayloadDocument.put("sdp", sdpAnswerString.c_str());
    sdpAnswerDocument.add_child("payload", sdpAnswerPayloadDocument);
    std::ostringstream sdpAnswerStream;
    pt::write_json(sdpAnswerStream, sdpAnswerDocument);
    signalingConnection->send(sdpAnswerStream.str());
}

void WebRTCConnection::OnBufferedAmountChange(uint64_t)
{

}

void WebRTCConnection::OnMessage(const webrtc::DataBuffer &buffer)
{
    Packet packet(this, std::string(buffer.data.data<char>(), buffer.data.size()));
    packetQueue->push(std::move(packet));
}

void WebRTCConnection::OnStateChange()
{

}

void WebRTCConnection::OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface>)
{

}

void WebRTCConnection::OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> channel)
{
    dataChannel = channel;
    dataChannel->RegisterObserver(this);
}

void WebRTCConnection::OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState)
{

}

void WebRTCConnection::OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState)
{

}

void WebRTCConnection::OnIceCandidate(const webrtc::IceCandidateInterface *candidate)
{
    std::string iceCandidateString;
    candidate->ToString(&iceCandidateString);
    pt::ptree iceCandidateDocument;
    iceCandidateDocument.put("type", "candidate");
    pt::ptree iceCandidatePayloadDocument;
    iceCandidatePayloadDocument.put("candidate", iceCandidateString.c_str());
    iceCandidatePayloadDocument.put("sdpMid", candidate->sdp_mid().c_str());
    iceCandidatePayloadDocument.put("sdpMLineIndex", candidate->sdp_mline_index());
    iceCandidateDocument.add_child("payload", iceCandidatePayloadDocument);
    std::ostringstream iceCandidateStream;
    pt::write_json(iceCandidateStream, iceCandidateDocument);
    signalingConnection->send(iceCandidateStream.str());
}

void WebRTCConnection::OnRemoveStream(rtc::scoped_refptr<webrtc::MediaStreamInterface>)
{

}

void WebRTCConnection::OnRenegotiationNeeded()
{

}

void WebRTCConnection::OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState)
{

}

void WebRTCConnection::OnSuccess()
{

}
