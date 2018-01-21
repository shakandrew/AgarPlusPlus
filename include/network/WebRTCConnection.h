#ifndef AGARPLUSPLUS_WEBRTCCONNECTION_H
#define AGARPLUSPLUS_WEBRTCCONNECTION_H

#include <memory>
#include <string>

#include <boost/lockfree/spsc_queue.hpp>
#include <webrtc/api/datachannelinterface.h>
#include <webrtc/api/peerconnectioninterface.h>

#include "Packet.h"
#include "WebRTCConfiguration.h"
#include "WebSocketConnection.h"

class Packet;

class WebRTCConnection : public std::enable_shared_from_this<WebRTCConnection>,
                         public webrtc::CreateSessionDescriptionObserver,
                         public webrtc::DataChannelObserver,
                         public webrtc::PeerConnectionObserver,
                         public webrtc::SetSessionDescriptionObserver
{
    WebRTCConfiguration *configuration;

    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> peerConnectionFactory;

    std::shared_ptr<WebSocketConnection> signalingConnection;

    boost::lockfree::spsc_queue<Packet> *packetQueue;

    rtc::scoped_refptr<webrtc::PeerConnectionInterface> peerConnection;

    rtc::scoped_refptr<webrtc::DataChannelInterface> dataChannel;

public:

    WebRTCConnection(WebRTCConfiguration *configuration,
                     rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> peerConnectionFactory,
                     std::shared_ptr<WebSocketConnection> signalingConnection,
                     boost::lockfree::spsc_queue<Packet> *packetQueue);

    void handleSignaling(std::string message);

    void run();

    void send(std::string const &message);

private:

    int AddRef() const override;

    int Release() const override;

    void OnFailure(const std::string&) override;

    void OnSuccess(webrtc::SessionDescriptionInterface* description) override;

    void OnBufferedAmountChange(uint64_t) override;

    void OnMessage(const webrtc::DataBuffer& buffer) override;

    void OnStateChange() override;

    void OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface>) override;

    void OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> channel) override;

    void OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState) override;

    void OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState) override;

    void OnIceCandidate(const webrtc::IceCandidateInterface* candidate) override;

    void OnRemoveStream(rtc::scoped_refptr<webrtc::MediaStreamInterface>) override;

    void OnRenegotiationNeeded() override;

    void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState) override;

    void OnSuccess() override;
};

#endif
