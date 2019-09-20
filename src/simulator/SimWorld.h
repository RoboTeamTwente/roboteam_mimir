//
// Created by rolf on 19-09-19.
//

#ifndef ROBOTEAM_MIMIR_SIMWORLD_H
#define ROBOTEAM_MIMIR_SIMWORLD_H

#include <bullet/btBulletDynamicsCommon.h>

class SimWorld {
public:
    SimWorld();
    ~SimWorld();
private:
    // these make up the total physics simulator together
    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* collisionDispatcher;
    btBroadphaseInterface* overlappingPairCache;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
};


#endif //ROBOTEAM_MIMIR_SIMWORLD_H
