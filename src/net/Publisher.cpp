//
// Created by rolf on 18-09-19.
//

#include "Publisher.h"

namespace net {
    // on construction we start the socket so it can send at any time
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

}