//
// Created by rolf on 19-09-19.
//

#ifndef ROBOTEAM_MIMIR_MOCKRECEIVER_H
#define ROBOTEAM_MIMIR_MOCKRECEIVER_H
#include <QHostAddress>
#include <QNetworkInterface>
#include "proto/messages_robocup_ssl_wrapper.pb.h"
#include "net/Publisher.h"

class mockReceiver{
private:
    QHostAddress IP;
    unsigned int port;
    QUdpSocket *socket;
public:
    mockReceiver(const QHostAddress& _address,unsigned int _port):
            port(_port),
            IP(_address){
        socket = new QUdpSocket;
        socket->bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
        //just join all possible network interfaces
        foreach (const QNetworkInterface &interface, QNetworkInterface::allInterfaces()) {
            socket->joinMulticastGroup(IP, interface);
        }
    }
    ~mockReceiver(){
        delete socket;
        socket=nullptr;
    }
    std::vector<SSL_WrapperPacket> readMessages(){
        std::vector<SSL_WrapperPacket> messages;
        while(socket->hasPendingDatagrams()){
            QByteArray datagram;
            datagram.resize(socket->pendingDatagramSize());
            socket->readDatagram(datagram.data(),datagram.size());
            SSL_WrapperPacket wrapper;
            if (wrapper.ParseFromArray(datagram.data(),datagram.size())){
                messages.push_back(wrapper);
            }
            else{
                throw "Cannot parse the packet correctly!";
            }
        }
        return messages;
    }

};

#endif //ROBOTEAM_MIMIR_MOCKRECEIVER_H
