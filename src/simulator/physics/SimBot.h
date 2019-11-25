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
#include <proto/mimir_robotcommand.pb.h>
class SimBall;
class SimBot : public BaseSimBot {
public:
    SimBot(unsigned int _id, std::shared_ptr<btDynamicsWorld> world, std::shared_ptr<RobotSettings> settings,
           std::shared_ptr<WorldSettings> worldSettings);
    SimBot(unsigned int _id, std::shared_ptr<btDynamicsWorld> world, const std::shared_ptr<RobotSettings> &settings,
           const std::shared_ptr<WorldSettings> &worldSettings, const btVector3 &initialPos, btScalar dir);
    ~SimBot();
    btVector3 position() const override;
    btScalar orientation() const override;

    void receiveCommand(const mimir_robotcommand &robotcommand, double time);
    void update(SimBall* ball,double time);
    unsigned int getId();
    void localControl(btScalar velTangent, btScalar velNormal, btScalar velAngle) override;
private:
    const unsigned int id;
    std::shared_ptr<btDynamicsWorld> dynamicsWorld;
    btAlignedObjectArray<btCollisionShape *> shapes;
    btRigidBody *body = nullptr;
    btDefaultMotionState *motionState = nullptr;
    btRigidBody *wheels[4];
    btHingeConstraint *wheelMotor[4];
    std::shared_ptr<RobotSettings> robSettings;

    void addWheels(const std::shared_ptr<RobotSettings> settings, const std::shared_ptr<WorldSettings> worldSettings,
                   btTransform hullTransform);
    void addWheel(int wheelLabel, btScalar wheelAngleD, btCollisionShape *wheelShape,
                  const std::shared_ptr<RobotSettings> settings, const std::shared_ptr<WorldSettings> worldSettings,
                  btTransform hullTransform);
    void wheelControl(btScalar wheel0, btScalar wheel1, btScalar wheel2, btScalar wheel3) override;
    void globalControl(btScalar xVel, btScalar yVel, btScalar angularVel);
    void globalControlAngle(btScalar xVel, btScalar yVel, btScalar angle);
    void deactivate();

    btHingeConstraint *dribblerMotor;
    btRigidBody *dribbler;



    mimir_robotcommand lastCommand;
    double lastCommandTime;
    bool isActive;

    bool kickerCharged=true;
    double lastKickTime;

    bool canKickBall(SimBall* Ball);

};


#endif //ROBOTEAM_MIMIR_SIMBOT_H
