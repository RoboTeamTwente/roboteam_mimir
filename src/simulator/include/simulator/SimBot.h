//
// Created by rolf on 13-10-19.
//

#ifndef ROBOTEAM_MIMIR_SIMBOT_H
#define ROBOTEAM_MIMIR_SIMBOT_H

#include "BaseSimBot.h"
#include "settings/RobotSettings.h"
#include "settings/WorldSettings.h"
#include "RobotMesh.h"
#include <memory>
#include <proto/mimir_robotcommand.pb.h>
#include <proto/messages_robocup_ssl_detection.pb.h>
#include <BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.h>
class SimBall;

class SimBotFrontEnd{
 public:
  SimBotFrontEnd(std::shared_ptr<btMultiBodyDynamicsWorld> world,const WorldSettings &worldSettings, const RobotSettings &settings,
                 btTransform robotHullTransform, btRigidBody * robotBody);

  void ballCollisionCallback(SimBall * ball,btVector3 collisionNormal);
 private:
  const double SCALE;
  std::shared_ptr<btMultiBodyDynamicsWorld> dynamicsWorld;
  btBoxShape * boxShape;
  btRigidBody * frontEndBody;
  btFixedConstraint * robotConstraint;

  double chargeKickTime = 1.0;
  double lastKickTime = -1000.0;
};

class SimBot : public BaseSimBot {
    public:
        SimBot(unsigned int _id, std::shared_ptr<btMultiBodyDynamicsWorld> world,
                const RobotSettings &settings,
                const WorldSettings &worldSettings);
        SimBot(unsigned int _id,  std::shared_ptr<btMultiBodyDynamicsWorld> world,
                const RobotSettings &settings,
                const WorldSettings &worldSettings,
                const btVector3 &initialPos,
                btScalar dir);
        ~SimBot();
        btVector3 position() const override;
        btScalar orientation() const override;
        btScalar height() const;

        void receiveCommand(const mimir_robotcommand &robotcommand, double time);
        void update(SimBall* ball, double time);
        unsigned int getId();
        SSL_DetectionRobot asDetection() const;
        void globalControl(btScalar xVel, btScalar yVel, btScalar angularVel);
    private:
        const unsigned int id;
        const double SCALE;
        std::shared_ptr<btMultiBodyDynamicsWorld> dynamicsWorld;
        btAlignedObjectArray<btCollisionShape*> shapes;
        btRigidBody* body = nullptr;
        btDefaultMotionState* motionState = nullptr;
        btRigidBody* wheels[4];
        btHingeConstraint* wheelMotor[4];
        RobotSettings robSettings;

        void addWheels(const WorldSettings &worldSettings,
                btTransform hullTransform);
        void addWheel(int wheelLabel, btScalar wheelAngleD, btCollisionShape* wheelShape,
                const WorldSettings &worldSettings,
                btTransform hullTransform);
        void wheelControl(btScalar wheel0, btScalar wheel1, btScalar wheel2, btScalar wheel3) override;
        void localControl(btScalar velTangent, btScalar velNormal, btScalar velAngle) override;
        void globalControlAngle(btScalar xVel, btScalar yVel, btScalar angle,btScalar dt);
        void deactivate();
        btScalar constrainAngle(btScalar angle);

        btScalar lastYaw = 0.0; // used in control: TODO initialize correctly
        SimBotFrontEnd * front_end;

        mimir_robotcommand lastCommand;
        double lastCommandTime = 0.0; //TODO: fix initialization
        bool isActive;

        bool kickerCharged = true;
        double lastKickTime;

        bool canKickBall(SimBall* Ball);
};

#endif //ROBOTEAM_MIMIR_SIMBOT_H
