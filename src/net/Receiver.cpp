//
// Created by rolf on 18-09-19.
//

#include "Receiver.h"
#include <QNetworkInterface>
//Receives all the robot commands
namespace net {
    Receiver::Receiver(const QHostAddress &_address, unsigned int _port) :
            IP(_address),
            port(_port),
            socket(nullptr) {
        start();// on creation we immediately start receiving
    }

    Receiver::~Receiver() {
        stop();
    }

    void Receiver::start() {
        stop();
        socket = new QUdpSocket(this);
        socket->bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
        //just join all possible network interfaces
        foreach (const QNetworkInterface &interface, QNetworkInterface::allInterfaces()) {
                socket->joinMulticastGroup(IP, interface);
            }
    }
    void Receiver::stop() {
        delete socket;
        socket = nullptr;
    }
    void Receiver::setIP(const QHostAddress &_address) {
        stop();
        IP=_address;
        start();
    }
    void Receiver::setPort(unsigned int _port) {
        stop();
        port=_port;
        start();
    }
    unsigned int Receiver::getPort() const {
        return port;
    }
    std::string Receiver::getIP() const {
        return IP.toString().toStdString();
    }
    std::vector<mimir_robotcommand> Receiver::readMessages() {
        std::vector<mimir_robotcommand> commands;
        while(socket->state()==QAbstractSocket::BoundState&&socket->hasPendingDatagrams()){
            QByteArray datagram;
            datagram.resize(socket->pendingDatagramSize());
            socket->readDatagram(datagram.data(),datagram.size());
            mimir_robotcommand command;
            if(command.ParseFromArray(datagram.data(),datagram.size())){
                commands.push_back(command);
            }
            else{
                //TODO: log errors
            }
        }
        return commands;
    }

}