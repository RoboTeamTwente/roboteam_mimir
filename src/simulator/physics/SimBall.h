//
// Created by rolf on 02-10-19.
//

#ifndef ROBOTEAM_MIMIR_SIMBALL_H
#define ROBOTEAM_MIMIR_SIMBALL_H

#include <btBulletDynamicsCommon.h>
#include <proto/messages_robocup_ssl_detection.pb.h>
#include "../config/WorldConfig.h"

class SimBall {
    public:
        SimBall(std::shared_ptr<btDynamicsWorld> _world, std::shared_ptr<WorldSettings> settings);
        SimBall(std::shared_ptr<btDynamicsWorld> _world, std::shared_ptr<WorldSettings> settings,
                const btVector3 &initialPos, const btVector3 &initialVel = btVector3(0.0f, 0.0f, 0.0f));
        ~SimBall();
        btVector3 position() const;
        btVector3 velocity() const;
        void kick(const btVector3 &force);
        SSL_DetectionBall asDetection() const;
    private:
        const double SCALE;
        std::shared_ptr<btDynamicsWorld> world;
        btSphereShape* physicsBall;
        btRigidBody* body;
        btMotionState* motionState;

};

#endif //ROBOTEAM_MIMIR_SIMBALL_H
