#ifndef OBSERVERS_H
#define OBSERVERS_H

#include <webrtc/api/peerconnectioninterface.h>

#include <functional>

class PeerConnectionObserver : public webrtc::PeerConnectionObserver
{
  public:
    PeerConnectionObserver(std::function<void(webrtc::DataChannelInterface*)> anOnDataChannel,
        std::function<void(const webrtc::IceCandidateInterface*)> anOnIceCandidate) :
        onDataChannel{anOnDataChannel}, onIceCandidate{anOnIceCandidate}
    {

    }

    void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState) override
    {

    }

    void OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface>) override
    {

    }

    void OnRemoveStream(rtc::scoped_refptr<webrtc::MediaStreamInterface>) override
    {

    }

    void OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> channel) override
    {
      onDataChannel(channel);
    }

    void OnRenegotiationNeeded() override
    {

    }

    void OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState) override
    {

    }

    void OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState) override
    {

    }

    void OnIceCandidate(const webrtc::IceCandidateInterface* candidate) override
    {
      onIceCandidate(candidate);
    }

  private:
    std::function<void(webrtc::DataChannelInterface*)> onDataChannel;
    std::function<void(const webrtc::IceCandidateInterface*)> onIceCandidate;
};

class DataChannelObserver : public webrtc::DataChannelObserver
{
  public:
    DataChannelObserver(std::function<void(const webrtc::DataBuffer&)> anOnMessage) :
        onMessage{anOnMessage}
    {

    }

    void OnStateChange() override
    {

    }

    void OnMessage(const webrtc::DataBuffer& buffer) override
    {
      onMessage(buffer);
    }

    void OnBufferedAmountChange(uint64_t) override
    {

    }

  private:
    std::function<void(const webrtc::DataBuffer&)> onMessage;
};

class CreateSessionDescriptionObserver : public webrtc::CreateSessionDescriptionObserver
{
  public:
    CreateSessionDescriptionObserver(std::function<void(webrtc::SessionDescriptionInterface*)>
        anOnSuccess) : onSuccess{anOnSuccess}
    {

    }

    void OnSuccess(webrtc::SessionDescriptionInterface* desc) override
    {
      onSuccess(desc);
    }

    void OnFailure(const std::string&) override
    {

    }

    int AddRef() const override
    {
        return 0;
    }

    int Release() const override
    {
        return 0;
    }

  private:
    std::function<void(webrtc::SessionDescriptionInterface*)> onSuccess;
};

class SetSessionDescriptionObserver : public webrtc::SetSessionDescriptionObserver
{
  public:
    SetSessionDescriptionObserver()
    {

    }

    void OnSuccess() override
    {

    }

    void OnFailure(const std::string&) override
    {

    }

    int AddRef() const override
    {
        return 0;
    }

    int Release() const override
    {
        return 0;
    }
};

#endif  // OBSERVERS_H