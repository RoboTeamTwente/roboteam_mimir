//
// Created by rolf on 13-10-19.
//

#ifndef ROBOTEAM_MIMIR_BASESIMBOT_H
#define ROBOTEAM_MIMIR_BASESIMBOT_H

#include "bullet/btBulletDynamicsCommon.h"

// A template class for any type of robot. Any class describing a robot object should inherit and implement the functions here
class BaseSimBot {
public:
    virtual btVector3 position() const = 0;
    virtual btScalar orientation() const = 0;
//    virtual void reset(btScalar x, btScalar y) = 0;
//    virtual void setDribbler(btScalar angularVel) = 0;

};


#endif //ROBOTEAM_MIMIR_BASESIMBOT_H
