//
// Created by rolf on 27-09-19.
//

#ifndef ROBOTEAM_MIMIR_SIMULATOR_H
#define ROBOTEAM_MIMIR_SIMULATOR_H

class SimWorld;
class ConfigWidget;
class btDiscreteDynamicsWorld;
namespace net{
    class Publisher;
    class Receiver;
}
class Simulator {
public:
    Simulator();
    ~Simulator();
    btDiscreteDynamicsWorld* getPhysicsWorld();
private:
    SimWorld* simWorld;
    ConfigWidget* configWidget;
    net::Publisher* publisher;
    net::Receiver* receiver;
};


#endif //ROBOTEAM_MIMIR_SIMULATOR_H
