//
// Created by rolf on 27-09-19.
//

#ifndef ROBOTEAM_MIMIR_SIMULATOR_H
#define ROBOTEAM_MIMIR_SIMULATOR_H

class SimWorld;
class ConfigWidget;
class btDiscreteDynamicsWorld;
class Simulator {
public:
    Simulator();
    ~Simulator();
    btDiscreteDynamicsWorld* getPhysicsWorld();
private:
    SimWorld* simWorld;
    ConfigWidget* configWidget;

};


#endif //ROBOTEAM_MIMIR_SIMULATOR_H
