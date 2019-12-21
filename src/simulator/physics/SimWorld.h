//
// Created by rolf on 19-09-19.
//

#ifndef ROBOTEAM_MIMIR_SIMWORLD_H
#define ROBOTEAM_MIMIR_SIMWORLD_H
//TODO: remove later
#include <QObject>
#include "iostream"
//end

#include <btBulletDynamicsCommon.h>
#include "proto/messages_robocup_ssl_wrapper.pb.h"
#include "proto/mimir_robotcommand.pb.h"
#include "SimBot.h"
#include "Camera.h"

class SimField;

class SimBall;

class RobotConfig;

class WorldConfig;

class Situation;

class SituationWorld;

class Random;

struct RobotCommand{
  RobotCommand(mimir_robotcommand command,double time) :
  command(std::move(command)),
  simulatorReceiveTime(time){}
  mimir_robotcommand command;
  double simulatorReceiveTime;
};
class SimWorld : public QObject {
    Q_OBJECT //TODO: figure out why this is necessary for compilation
    public:
        SimWorld(const std::unique_ptr<WorldConfig>& _worldSettings, const std::unique_ptr<RobotConfig>& _blueSettings,
                const std::unique_ptr<RobotConfig>& _yellowSettings, const std::unique_ptr<SituationWorld>& _situation);
        ~SimWorld() override;
        btDiscreteDynamicsWorld* getWorld();
        std::vector<SSL_WrapperPacket> getPackets();
        void doCommands(btScalar dt);
        void addCommands(const std::vector<mimir_robotcommand>& commands, bool TeamIsYellow); //TODO: fix copying
        void setRobotCount(unsigned numRobots, bool isYellow);
        void updateWorldConfig(const std::unique_ptr<WorldConfig>& _worldSettings);
        void updateRobotConfig(const std::unique_ptr<RobotConfig>& _robotSettings, bool isYellow);
        WorldSettings *getWorldSettings();
        RobotSettings *getRobotSettings(bool isYellow);
        void setSendGeometryTicks(unsigned int ticks);
        SSL_GeometryData getGeometryData();
        void stepSimulation(double dt);
        void setDelay(double _delay);

        void setRobotXNoise(double noise);
        void setRobotYNoise(double noise);
        void setRobotOrNoise(double noise);
        void setRobotVanishing(double prob);

        void setBallXNoise(double noise);
        void setBallYNoise(double noise);
        void setBallVanishing(double prob);

private:
        void resetRobots();
        void resetWorld();
        void reloadSituation();
        std::vector<SSL_DetectionFrame> getDetectionFrames();

        std::unique_ptr<SimField> field;
        std::unique_ptr<SimBall> ball;
        std::vector<std::unique_ptr<SimBot>> blueBots;
        std::vector<std::unique_ptr<SimBot>> yellowBots;
        std::vector<Camera> cameras;
        std::vector<RobotCommand> blueCommands;
        std::vector<RobotCommand> yellowCommands;
        // these make up the total physics simulator together
        std::unique_ptr<btDefaultCollisionConfiguration> collisionConfig;
        std::unique_ptr<btCollisionDispatcher> collisionDispatcher;
        std::unique_ptr<btBroadphaseInterface> overlappingPairCache;
        std::unique_ptr<btConstraintSolver> solver;
        std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld; // is publicly accessible through getWorld() for debugDrawing purposes

        std::unique_ptr<RobotSettings> blueSettings;
        std::unique_ptr<RobotSettings> yellowSettings;
        std::unique_ptr<WorldSettings> worldSettings;
        std::unique_ptr<SituationWorld> situation;

        std::unique_ptr<Random> random;
        double ballVanishingProb;
        double robotVanishingProb;
        double delay;

        unsigned int numBlueBots = 1;
        unsigned int numYellowBots = 1;
        unsigned int sendGeometryTicks = 120;
        int tickCount = 0;
        double time = 0;

};

#endif //ROBOTEAM_MIMIR_SIMWORLD_H
