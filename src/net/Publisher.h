//
// Created by rolf on 18-09-19.
//

#ifndef ROBOTEAM_MIMIR_PUBLISHER_H
#define ROBOTEAM_MIMIR_PUBLISHER_H

#include <QUdpSocket>

namespace net {
    class Publisher : public QObject {
    Q_OBJECT
    public:
        Publisher(const QHostAddress &_address, unsigned int _port);
        ~Publisher() override;
        Publisher(const Publisher &) = delete;
        Publisher &operator=(const Publisher &) = delete;

        std::string getIP() const;
        unsigned int getPort() const;
    public slots:
        void setIP(const QHostAddress &_address);
        void setPort(unsigned int _port);
    private:
        unsigned int port;
        QHostAddress IP;
        QUdpSocket *socket;
    };
}


#endif //ROBOTEAM_MIMIR_PUBLISHER_H
