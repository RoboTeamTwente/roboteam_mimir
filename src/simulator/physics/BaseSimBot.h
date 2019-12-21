//
// Created by rolf on 13-10-19.
//

#ifndef ROBOTEAM_MIMIR_BASESIMBOT_H
#define ROBOTEAM_MIMIR_BASESIMBOT_H

#include <btBulletDynamicsCommon.h>

// A template class for any type of robot. Any class describing a robot object should inherit and implement the functions here
class BaseSimBot {
    public:
        virtual btVector3 position() const = 0;
        virtual btScalar orientation() const = 0;
        virtual void wheelControl(btScalar wheel0, btScalar wheel1, btScalar wheel2, btScalar wheel3) = 0;
        virtual void localControl(btScalar velTangent, btScalar velNormal, btScalar velAngle) = 0;
//    virtual void setDribbler(btScalar angularVel) = 0;

};

#endif //ROBOTEAM_MIMIR_BASESIMBOT_H
