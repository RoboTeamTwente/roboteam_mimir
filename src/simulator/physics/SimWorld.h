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

class SimField;

class SimBall;

class RobotSettings;

class WorldSettings;

class SimWorld : public QObject {
Q_OBJECT
public slots:
    void stepSimulation();
public:
    SimWorld(WorldSettings *_worldSettings, RobotSettings *_blueSettings, RobotSettings *_yellowSettings);
    ~SimWorld() override;
    btDiscreteDynamicsWorld *getWorld();
    SSL_GeometryData getGeometryData();
    SSL_DetectionFrame getDetectionFrames();
private:
    SimField *field;
    SimBall *ball;
    // these make up the total physics simulator together
    btDefaultCollisionConfiguration *collisionConfig;
    btCollisionDispatcher *collisionDispatcher;
    btBroadphaseInterface *overlappingPairCache;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *dynamicsWorld; // is publicly accessible through getWorld() for debugDrawing purposes
    // we create a local copy of the settings
    RobotSettings *blueSettings = nullptr;
    RobotSettings *yellowSettings = nullptr;
    WorldSettings *worldSettings = nullptr;
};


#endif //ROBOTEAM_MIMIR_SIMWORLD_H
