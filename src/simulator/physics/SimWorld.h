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
class ConfigWidget;

class SimWorld : public QObject {
    Q_OBJECT
public slots:
    void stepSimulation();
public:
    SimWorld();
    ~SimWorld();
    btDiscreteDynamicsWorld* getWorld();
private:
    QTimer * timer; //TODO create better timer
    ConfigWidget * widget; //TODO remove after testing and make proper infrastructure
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
