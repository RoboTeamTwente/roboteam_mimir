//
// Created by rolf on 18-09-19.
//

#include "Publisher.h"

namespace net {
    // on construction we start the socket so it can send at any time we call it.
    Publisher::Publisher(const QHostAddress &_address, const unsigned int _port) :
            port(_port),
            IP(_address),
            socket(new QUdpSocket(this)) {}

    Publisher::~Publisher() {
        delete socket;
        socket=nullptr;
    }
    // returns the current IP address the publisher is sending to
    std::string Publisher::getIP() const {
        return IP.toString().toStdString();
    }

    // returns the current port the publisher is sending to
    unsigned int Publisher::getPort() const {
        return port;
    }

    void Publisher::setIP(const QHostAddress &_address) {
        IP = _address;
    }

    void Publisher::setPort(const unsigned int _port) {
        port = _port;
    }

    //returns true if packet is sent correctly.
    //TODO: needs logging
    bool Publisher::send(const SSL_WrapperPacket &packet) {
        QByteArray datagram;

        datagram.resize(packet.ByteSize());
        bool validSerialization=packet.SerializeToArray(datagram.data(),datagram.size());
        if (!validSerialization){
            return false;
        }

        qint64 bytesSent=socket->writeDatagram(datagram,IP,port);
        if (bytesSent!=datagram.size()){
            return false;
        }
        return true;
    }
    bool Publisher::send(const SSL_GeometryData &data) {
        SSL_WrapperPacket packet;
        packet.mutable_geometry()->CopyFrom(data);
        return send(packet);
    }
    bool Publisher::send(const SSL_DetectionFrame &frame) {
        SSL_WrapperPacket packet;
        packet.mutable_detection()->CopyFrom(frame);
        return send(packet);
    }
}