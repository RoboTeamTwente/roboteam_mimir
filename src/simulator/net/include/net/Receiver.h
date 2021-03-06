//
// Created by rolf on 18-09-19.
//

#ifndef ROBOTEAM_MIMIR_RECEIVER_H
#define ROBOTEAM_MIMIR_RECEIVER_H

#include <QUdpSocket>
#include "proto/mimir_robotcommand.pb.h"
namespace net {
    class Receiver : public QObject {
    Q_OBJECT
    public:
        Receiver(const QHostAddress &_address, unsigned int _port);
        ~Receiver() override;
        Receiver(const Receiver &) = delete;
        Receiver &operator=(const Receiver &) = delete;
        std::string getIP() const;
        unsigned int getPort() const;
        void start();
        void stop();
    public slots:
        void setIP(const QHostAddress &_address);
        void setPort(unsigned int _port);

        std::vector<mimir_robotcommand> readMessages();
    private:
        unsigned int port;
        QHostAddress IP;
        QUdpSocket *socket;
    };
}


#endif //ROBOTEAM_MIMIR_RECEIVER_H
