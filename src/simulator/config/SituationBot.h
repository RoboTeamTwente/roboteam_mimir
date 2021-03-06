//
// Created by rolf on 07-12-19.
//

#ifndef ROBOTEAM_MIMIR_SITUATIONBOT_H
#define ROBOTEAM_MIMIR_SITUATIONBOT_H

#include "btBulletDynamicsCommon.h"

class SituationBot {
public:
    SituationBot(unsigned int _id, double x, double y, double ang, double velX, double velY, double velAng);
    const unsigned int id;
    const btVector3 position;
    const btVector3 velocity;
};


#endif //ROBOTEAM_MIMIR_SITUATIONBOT_H
