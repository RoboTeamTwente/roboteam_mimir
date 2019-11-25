//
// Created by rolf on 19-09-19.
//

#ifndef ROBOTEAM_MIMIR_SIMWORLD_H
#define ROBOTEAM_MIMIR_SIMWORLD_H
//TODO: remove later
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
    void addCommands(std::vector<mimir_robotcommand> commands,bool TeamIsYellow); //TODO: fix copying
    void setRobotCount(unsigned int robotsPerTeam);
private:
    void resetRobots();
    SSL_GeometryData getGeometryData();
    std::vector<SSL_DetectionFrame> getDetectionFrames();

    std::shared_ptr<SimField> field;
    std::shared_ptr<SimBall> ball;
    std::vector<std::unique_ptr<SimBot>> blueBots;
    std::vector<std::unique_ptr<SimBot>> yellowBots;

    std::vector<mimir_robotcommand> blueCommands;
    std::vector<mimir_robotcommand> yellowCommands;
    // these make up the total physics simulator together
    std::unique_ptr<btDefaultCollisionConfiguration> collisionConfig;
    std::unique_ptr<btCollisionDispatcher> collisionDispatcher;
    std::unique_ptr<btBroadphaseInterface> overlappingPairCache;
    std::unique_ptr<btConstraintSolver> solver;
    std::shared_ptr<btDiscreteDynamicsWorld> dynamicsWorld; // is publicly accessible through getWorld() for debugDrawing purposes
    // we create a local copy of the settings
    std::shared_ptr<RobotSettings> blueSettings = nullptr;
    std::shared_ptr<RobotSettings> yellowSettings = nullptr; //TODO: unused?
    std::shared_ptr<WorldSettings> worldSettings = nullptr;

    unsigned int numRobots = 1;
    int tickCount = 0;
    double time = 0;

};


#endif //ROBOTEAM_MIMIR_SIMWORLD_H
