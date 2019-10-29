//
// Created by rolf on 13-10-19.
//

#ifndef ROBOTEAM_MIMIR_SIMBOT_H
#define ROBOTEAM_MIMIR_SIMBOT_H

#include "BaseSimBot.h"
#include "../config/RobotSettings.h"
#include "../config/WorldSettings.h"
#include "RobotMesh.h"
#include <memory>

class SimBot : public BaseSimBot {
public:
    SimBot(btDynamicsWorld * world, std::shared_ptr<RobotSettings> settings, std::shared_ptr<WorldSettings> worldSettings);
    SimBot(btDynamicsWorld * world, std::shared_ptr<RobotSettings> settings, std::shared_ptr<WorldSettings> worldSettings, const btVector3& initialPos,btScalar dir);
    ~SimBot();
    btVector3 position() const override;
    btScalar orientation() const override;
//    void reset(btScalar x,btScalar y) override;
private:

    btDynamicsWorld * dynamicsWorld;
    btAlignedObjectArray<btCollisionShape*> shapes;
    btRigidBody* body=nullptr;
    btDefaultMotionState * motionState=nullptr;
    btRigidBody* wheels[4];
    btHingeConstraint* wheelMotor[4];
    void addWheels(const std::shared_ptr<RobotSettings> settings, const std::shared_ptr<WorldSettings> worldSettings,btTransform hullTransform);
    void addWheel(int wheelLabel,btScalar wheelAngleD,btCollisionShape * wheelShape,const std::shared_ptr<RobotSettings> settings, const std::shared_ptr<WorldSettings> worldSettings,btTransform hullTransform);

};


#endif //ROBOTEAM_MIMIR_SIMBOT_H
