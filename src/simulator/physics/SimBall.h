//
// Created by rolf on 02-10-19.
//

#ifndef ROBOTEAM_MIMIR_SIMBALL_H
#define ROBOTEAM_MIMIR_SIMBALL_H

#include <btBulletDynamicsCommon.h>
#include "../config/WorldConfig.h"

class SimBall {
public:
    SimBall(btDynamicsWorld* _world,WorldSettings* settings);
    SimBall(btDynamicsWorld* _world,WorldSettings* settings,const btVector3& initialPos, const btVector3& initialVel = btVector3(0.0f,0.0f,0.0f));
    ~SimBall();
    btVector3 position();//TODO make const again
    btVector3 velocity() const;
private:
    int tick=0;
    float lastVel=0.0;
    btDynamicsWorld * world;
    btCollisionShape * physicsBall;
    btRigidBody * body;
    btMotionState * motionState;

};


#endif //ROBOTEAM_MIMIR_SIMBALL_H
