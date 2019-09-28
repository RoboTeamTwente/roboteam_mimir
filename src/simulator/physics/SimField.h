//
// Created by rolf on 28-09-19.
//

#ifndef ROBOTEAM_MIMIR_SIMFIELD_H
#define ROBOTEAM_MIMIR_SIMFIELD_H

#include <bullet/btBulletDynamicsCommon.h>
class SimField {
public:
    explicit SimField(btDynamicsWorld *world);
    ~SimField();
private:
    btDynamicsWorld * dynamicsWorld;
    btAlignedObjectArray<btCollisionObject*> objects;
    btStaticPlaneShape * ground;
    void addObject(btCollisionShape *shape, const btTransform &transform);

};


#endif //ROBOTEAM_MIMIR_SIMFIELD_H