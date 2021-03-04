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
        virtual ~SimBall();
        [[nodiscard]] btVector3 position() const;
        [[nodiscard]] btVector3 velocity() const;
        [[nodiscard]] btScalar radius() const;

        void instantKick(const btVector3& velocity);
        void kick(const btVector3 &force);
        void processKicks(double dt);

        void dribble(const btVector3& torque);
        [[nodiscard]] SSL_DetectionBall asDetection() const;
    protected:
        const double SCALE;
        btMultiBody* multiBody = nullptr;

    private:
        std::shared_ptr<btMultiBodyDynamicsWorld> world;
        btSphereShape* physicsBall = nullptr;
        btMotionState* motionState = nullptr;

        bool isKicked = false;
        btVector3 kickedForce = btVector3(0,0,0);
};

#endif //ROBOTEAM_MIMIR_SIMBALL_H
