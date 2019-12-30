//
// Created by rolf on 28-09-19.
//

#ifndef ROBOTEAM_MIMIR_SIMFIELD_H
#define ROBOTEAM_MIMIR_SIMFIELD_H

#include <btBulletDynamicsCommon.h>
#include "../config/WorldSettings.h"
#include <memory>
#include <BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.h>

class SimField {
public:
    explicit SimField(std::unique_ptr <btMultiBodyDynamicsWorld>& world, const std::unique_ptr<WorldSettings>& cfg);
    ~SimField();
private:
    std::unique_ptr<btMultiBodyDynamicsWorld>& dynamicsWorld;
    btAlignedObjectArray<btCollisionObject*> objects;
    btStaticPlaneShape * plane;
    btCollisionShape * goalBack;
    btCollisionShape * goalSide;
    void addObject(btCollisionShape *shape, const btTransform &transform);

};


#endif //ROBOTEAM_MIMIR_SIMFIELD_H
