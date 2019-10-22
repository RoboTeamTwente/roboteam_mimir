//
// Created by rolf on 13-10-19.
//

#ifndef ROBOTEAM_MIMIR_SIMBOT_H
#define ROBOTEAM_MIMIR_SIMBOT_H

#include "BaseSimBot.h"
#include "../config/RobotSettings.h"
#include "../config/WorldSettings.h"
#include "RobotMesh.h"

class SimBot : public BaseSimBot {
public:
    SimBot(btDynamicsWorld * world, RobotSettings* settings, WorldSettings* worldSettings);
    SimBot(btDynamicsWorld * world, RobotSettings* settings, WorldSettings* worldSettings, const btVector3& initialPos,btScalar dir);
    ~SimBot();
    btVector3 position() const override;
    btScalar orientation() const override;
//    void reset(btScalar x,btScalar y) override;
private:

    btDynamicsWorld * dynamicsWorld;
    btAlignedObjectArray<btCollisionShape*> shapes;
    btRigidBody* body=nullptr;
    btDefaultMotionState * motionState=nullptr;
    void addWheels(const RobotSettings *settings, const WorldSettings *worldSettings);
    void addWheel(int wheelLabel,btScalar wheelAngleD,btCollisionShape * wheelShape,const RobotSettings *settings, const WorldSettings *worldSettings);

};


#endif //ROBOTEAM_MIMIR_SIMBOT_H
