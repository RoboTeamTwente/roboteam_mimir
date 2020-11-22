//
// Created by rolf on 02-10-19.
//

#ifndef ROBOTEAM_MIMIR_SIMBALL_H
#define ROBOTEAM_MIMIR_SIMBALL_H

#include <btBulletDynamicsCommon.h>
#include <proto/messages_robocup_ssl_detection.pb.h>
#include <BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.h>
#include <BulletDynamics/Featherstone/btMultiBodyLinkCollider.h>
#include "settings/WorldSettings.h"
#include "BulletDynamics/Featherstone/btMultiBody.h"

class SimBall {
    public:
        SimBall(std::shared_ptr <btMultiBodyDynamicsWorld> _world, const WorldSettings &settings);
        SimBall(std::shared_ptr <btMultiBodyDynamicsWorld> _world, const WorldSettings &settings,
                const btVector3 &initialPos, const btVector3 &initialVel = btVector3(0.0f, 0.0f, 0.0f));
        ~SimBall();
        [[nodiscard]] btVector3 position() const;
        [[nodiscard]] btVector3 velocity() const;
        [[nodiscard]] btScalar radius() const;
        void kick(const btVector3 &force);
        [[nodiscard]] SSL_DetectionBall asDetection() const;
    private:
        const double SCALE;
        std::shared_ptr<btMultiBodyDynamicsWorld> world;
        btSphereShape* physicsBall;
        btMultiBody* multiBody;
        btMotionState* motionState;

};

#endif //ROBOTEAM_MIMIR_SIMBALL_H
