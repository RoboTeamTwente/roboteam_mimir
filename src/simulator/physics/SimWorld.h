//
// Created by rolf on 19-09-19.
//

#ifndef ROBOTEAM_MIMIR_SIMWORLD_H
#define ROBOTEAM_MIMIR_SIMWORLD_H

#include <bullet/btBulletDynamicsCommon.h>
#include "../ConfigWidget.h"
class SimField;
class SimBall;
class SimWorld {
public:
    SimWorld();
    ~SimWorld();
    btDiscreteDynamicsWorld* getWorld();
private:
    ConfigWidget * widget; //TODO remove after testing and make proper infrastructure
    SimField * field;
    SimBall * ball;
    // these make up the total physics simulator together
    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* collisionDispatcher;
    btBroadphaseInterface* overlappingPairCache;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld; // is publicly accessible through getWorld() for debugDrawing purposes
};


#endif //ROBOTEAM_MIMIR_SIMWORLD_H
