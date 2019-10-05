//
// Created by rolf on 27-09-19.
//

#ifndef ROBOTEAM_MIMIR_SIMULATOR_H
#define ROBOTEAM_MIMIR_SIMULATOR_H

#include <QObject>

class SimWorld;
class ConfigWidget;
class btDiscreteDynamicsWorld;
namespace net{
    class Publisher;
    class Receiver;
}
class QTimer;
class Simulator :public QObject {
    Q_OBJECT
public:
    Simulator();
    ~Simulator();
    btDiscreteDynamicsWorld* getPhysicsWorld();
public slots:
    void tick();
private:
    QTimer *timer;
    SimWorld* simWorld;
    ConfigWidget* configWidget;
    net::Publisher* publisher;
    net::Receiver* receiver;
};


#endif //ROBOTEAM_MIMIR_SIMULATOR_H
