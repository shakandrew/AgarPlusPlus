const webSocketUrl = 'ws://192.168.60.211:2239';
let webSocketConnection = null;
let rtcPeerConnection = null;
let dataChannel = null;

function onDataChannelOpen() {
    console.log('Data channel is open.');
}

function onDataChannelMessage(event) {
    console.log(event.data);
}

function onIceCandidate(event) {
    if (event && event.candidate) {
        webSocketConnection.send(JSON.stringify({type: 'candidate', payload: event.candidate}));
    }
}

function onOfferCreated(description) {
    rtcPeerConnection.setLocalDescription(description);
    webSocketConnection.send(JSON.stringify({type: 'offer', payload: description}));
}

function onWebSocketOpen() {
    const config = {
        iceServers: [
            {
                urls: [
                    'stun:stun.l.google.com:19302'
                ]
            }
        ]
    };
    rtcPeerConnection = new RTCPeerConnection(config);
    const dataChannelConfig = {
        ordered: false,
        maxRetransmits: 0
    };
    dataChannel = rtcPeerConnection.createDataChannel('dc', dataChannelConfig);
    dataChannel.onmessage = onDataChannelMessage;
    dataChannel.onopen = onDataChannelOpen;
    const sdpConstraints = {
        mandatory: {
            OfferToReceiveAudio: false,
            OfferToReceiveVideo: false
        }
    };
    rtcPeerConnection.onicecandidate = onIceCandidate;
    rtcPeerConnection.createOffer(onOfferCreated, () => {}, sdpConstraints);
}

function onWebSocketMessage(event) {
    const messageObject = JSON.parse(event.data);
    if (messageObject.type === 'answer') {
        rtcPeerConnection.setRemoteDescription(new RTCSessionDescription(messageObject.payload));
    } else if (messageObject.type === 'candidate') {
        rtcPeerConnection.addIceCandidate(new RTCIceCandidate(messageObject.payload));
    } else {
        console.log('Unknown message type.');
    }
}

function connect() {
    webSocketConnection = new WebSocket(webSocketUrl);
    webSocketConnection.onopen = onWebSocketOpen;
    webSocketConnection.onmessage = onWebSocketMessage;
}

function testMessage() {

    dataChannel.send(Date.now());
}