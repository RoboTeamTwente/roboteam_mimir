//
// Created by rolf on 27-09-19.
//

#ifndef ROBOTEAM_MIMIR_SIMULATOR_H
#define ROBOTEAM_MIMIR_SIMULATOR_H

#include <QObject>
#include <memory>

class SimWorld;
class ConfigWidget;
class btDiscreteDynamicsWorld;
class WorldSettings;
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
    std::shared_ptr<WorldSettings> getWorldSettings();
public slots:
    void tick();
private:
    QTimer *timer;
    SimWorld* simWorld;
    ConfigWidget* configWidget;
    std::unique_ptr<net::Publisher> publisher;
    std::unique_ptr<net::Receiver> blueReceiver;
    std::unique_ptr<net::Receiver> yellowReceiver;

};


#endif //ROBOTEAM_MIMIR_SIMULATOR_H
