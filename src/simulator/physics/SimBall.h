//
// Created by rolf on 02-10-19.
//

#ifndef ROBOTEAM_MIMIR_SIMBALL_H
#define ROBOTEAM_MIMIR_SIMBALL_H

#include <btBulletDynamicsCommon.h>
#include <proto/messages_robocup_ssl_detection.pb.h>
#include <BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.h>
#include <BulletDynamics/Featherstone/btMultiBodyLinkCollider.h>
#include "BulletDynamics/Featherstone/btMultiBody.h"
#include "../config/WorldConfig.h"

class SimBall {
    public:
        SimBall(std::unique_ptr <btMultiBodyDynamicsWorld>& _world, const std::unique_ptr<WorldSettings> &settings);
        SimBall(std::unique_ptr <btMultiBodyDynamicsWorld>& _world, const std::unique_ptr<WorldSettings> &settings,
                const btVector3 &initialPos, const btVector3 &initialVel = btVector3(0.0f, 0.0f, 0.0f));
        ~SimBall();
        btVector3 position() const;
        btVector3 velocity() const;
        void kick(const btVector3 &force);
        SSL_DetectionBall asDetection() const;
    private:
        const double SCALE;
        std::unique_ptr<btMultiBodyDynamicsWorld>& world;
        btSphereShape* physicsBall;
        btMultiBody* multiBody;
        btMotionState* motionState;

};

#endif //ROBOTEAM_MIMIR_SIMBALL_H
