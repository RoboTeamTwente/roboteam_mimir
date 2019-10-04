//
// Created by rolf on 19-09-19.
//

#ifndef ROBOTEAM_MIMIR_SIMWORLD_H
#define ROBOTEAM_MIMIR_SIMWORLD_H
//remove later
#include <QTimer>
#include <QObject>
#include "iostream"
//end

#include <bullet/btBulletDynamicsCommon.h>
class SimField;
class SimBall;
class RobotSettings;
class WorldSettings;

class SimWorld : public QObject {
    Q_OBJECT
public slots:
    void stepSimulation();
public:
    SimWorld(WorldSettings* worldSettings,RobotSettings* blueSettings,RobotSettings* yellowSettings);
    ~SimWorld();
    btDiscreteDynamicsWorld* getWorld();
private:
    QTimer * timer; //TODO create better timer
    SimField * field;
    SimBall * ball;
    // these make up the total physics simulator together
    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* collisionDispatcher;
    btBroadphaseInterface* overlappingPairCache;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld; // is publicly accessible through getWorld() for debugDrawing purposes
};


#endif //ROBOTEAM_MIMIR_SIMWORLD_H
