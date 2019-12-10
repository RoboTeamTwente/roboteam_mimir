//
// Created by rolf on 07-12-19.
//

#ifndef ROBOTEAM_MIMIR_SITUATIONBALL_H
#define ROBOTEAM_MIMIR_SITUATIONBALL_H

#include "btBulletDynamicsCommon.h"
class SituationBall {
public:
    /*
     * Note the ball is assumed to be at it's radius height
     */
    SituationBall(double x, double y, double xVel, double yVel);
    SituationBall(double x, double y, double z, double xVel, double yVel, double zVel);
    const btVector3 position;
    const btVector3 velocity;
    const bool isRolling;
};


#endif //ROBOTEAM_MIMIR_SITUATIONBALL_H
