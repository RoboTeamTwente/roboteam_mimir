//
// Created by rolf on 19-09-19.
//

#ifndef ROBOTEAM_MIMIR_SIMWORLD_H
#define ROBOTEAM_MIMIR_SIMWORLD_H
//remove later
#include <QObject>
#include "iostream"
//end

#include <bullet/btBulletDynamicsCommon.h>
#include "proto/messages_robocup_ssl_wrapper.pb.h"
#include "proto/mimir_robotcommand.pb.h"
#include "SimBot.h"

class SimField;

class SimBall;

class RobotSettings;

class WorldSettings;

class SimWorld : public QObject {
Q_OBJECT
public slots:
    void stepSimulation();
public:
    SimWorld(std::shared_ptr<WorldSettings> _worldSettings, std::shared_ptr<RobotSettings>_blueSettings, std::shared_ptr<RobotSettings>_yellowSettings);
    ~SimWorld() override;
    btDiscreteDynamicsWorld *getWorld();
    std::vector<SSL_WrapperPacket> getPackets();
    void doCommands(btScalar dt);
private:
    SSL_GeometryData getGeometryData();
    std::vector<SSL_DetectionFrame> getDetectionFrames();

    std::shared_ptr<SimField> field;
    std::shared_ptr<SimBall> ball;
    std::shared_ptr<SimBot> test;//TODO: remove
    std::vector<std::shared_ptr<SimBot>> blueBots;
    std::vector<std::shared_ptr<SimBot>> yellowBots;
    // these make up the total physics simulator together
    btDefaultCollisionConfiguration *collisionConfig;
    btCollisionDispatcher *collisionDispatcher;
    btBroadphaseInterface *overlappingPairCache;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *dynamicsWorld; // is publicly accessible through getWorld() for debugDrawing purposes
    // we create a local copy of the settings
    std::shared_ptr<RobotSettings> blueSettings = nullptr;
    std::shared_ptr<RobotSettings> yellowSettings = nullptr;
    std::shared_ptr<WorldSettings> worldSettings = nullptr;

    int tickCount = 0;

};


#endif //ROBOTEAM_MIMIR_SIMWORLD_H
