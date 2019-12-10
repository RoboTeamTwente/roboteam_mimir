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
#include "Camera.h"
#include <random>

class SimField;

class SimBall;

class RobotConfig;

class WorldConfig;

class SimWorld : public QObject {
    Q_OBJECT
    public slots:
        void stepSimulation();
    public:
        SimWorld(const std::unique_ptr<WorldConfig>& _worldSettings, const std::unique_ptr<RobotConfig>& _blueSettings,
                const std::unique_ptr<RobotConfig>& _yellowSettings);
        ~SimWorld() override;
        btDiscreteDynamicsWorld* getWorld();
        std::vector<SSL_WrapperPacket> getPackets();
        void doCommands(btScalar dt);
        void addCommands(std::vector<mimir_robotcommand> commands, bool TeamIsYellow); //TODO: fix copying
        void setRobotCount(unsigned numRobots, bool isYellow);
        void updateWorldConfig(const std::unique_ptr<WorldConfig>& _worldSettings);
        void updateRobotConfig(const std::unique_ptr<RobotConfig>& _robotSettings, bool isYellow);
        WorldSettings *getWorldSettings();
        RobotSettings *getRobotSettings(bool isYellow);
        void setSendGeometryTicks(unsigned int ticks);
        void resetWorld();
        SSL_GeometryData getGeometryData();

    private:
        void resetRobots();
        std::vector<SSL_DetectionFrame> getDetectionFrames();

        std::shared_ptr<SimField> field;
        std::shared_ptr<SimBall> ball;
        std::vector<std::unique_ptr<SimBot>> blueBots;
        std::vector<std::unique_ptr<SimBot>> yellowBots;
        std::vector<Camera> cameras;
        std::vector<mimir_robotcommand> blueCommands;
        std::vector<mimir_robotcommand> yellowCommands;
        // these make up the total physics simulator together
        std::unique_ptr<btDefaultCollisionConfiguration> collisionConfig;
        std::unique_ptr<btCollisionDispatcher> collisionDispatcher;
        std::unique_ptr<btBroadphaseInterface> overlappingPairCache;
        std::unique_ptr<btConstraintSolver> solver;
        std::shared_ptr<btDiscreteDynamicsWorld> dynamicsWorld; // is publicly accessible through getWorld() for debugDrawing purposes

        std::unique_ptr<RobotSettings> blueSettings;
        std::unique_ptr<RobotSettings> yellowSettings;
        std::unique_ptr<WorldSettings> worldSettings;

    //TODO: perhaps make this it's own class.
        double getRandomUniform();
        std::mt19937 randomGenerator;
        std::uniform_real_distribution<double> uniformDist;

        unsigned int numBlueBots = 1;
        unsigned int numYellowBots = 1;
        unsigned int sendGeometryTicks = 120;
        int tickCount = 0;
        double time = 0;

};

#endif //ROBOTEAM_MIMIR_SIMWORLD_H
