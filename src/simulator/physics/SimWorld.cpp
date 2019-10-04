//
// Created by rolf on 19-09-19.
//

// The following is a great manual giving lots of tips hints and explanations on the library
// http://www.cs.kent.edu/~ruttan/GameEngines/lectures/Bullet_User_Manual
// this resource also has a lot of information:
// https://docs.google.com/document/d/10sXEhzFRSnvFcl3XxNGhnD4N2SedqwdAvK3dsihxVUA/edit#heading=h.2ye70wns7io3

#include "SimWorld.h"
#include "SimField.h"
#include "SimBall.h"
#include "../config/WorldSettings.h"
#include "../config/RobotSettings.h"

SimWorld::SimWorld(WorldSettings* worldSettings,RobotSettings* blueSettings,RobotSettings* yellowSettings) {
    //Contains default setup for memory and how collisions between different types of objects are handled/calculated
    collisionConfig = new btDefaultCollisionConfiguration();

    //uses the default dispatcher. We might want to use the parallel one down the road.
    collisionDispatcher = new btCollisionDispatcher(collisionConfig);

    //btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    overlappingPairCache = new btDbvtBroadphase();

    //the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    solver = new btSequentialImpulseConstraintSolver();

    // the world in which all simulation happens
    dynamicsWorld= new btDiscreteDynamicsWorld(collisionDispatcher,overlappingPairCache,solver,collisionConfig);
    dynamicsWorld->setGravity(btVector3(worldSettings->gravityX,worldSettings->gravityY,worldSettings->gravityZ));

    //field creates and manages all of the geometry related (static) physics objects in the world
    field=new SimField(dynamicsWorld,worldSettings);
    //create a ball
    ball=new SimBall(dynamicsWorld,worldSettings);

    //start ticking the world
    timer= new QTimer();
    timer->setTimerType(Qt::PreciseTimer);
    connect(timer,&QTimer::timeout,this,&SimWorld::stepSimulation);
    timer->start(16);
}
SimWorld::~SimWorld() {
    //delete everything in reverse order of creation!
    delete field;
    delete dynamicsWorld;
    delete solver;
    delete overlappingPairCache;
    delete collisionDispatcher;
    delete collisionConfig;
}
btDiscreteDynamicsWorld* SimWorld::getWorld() {
    return dynamicsWorld;
}
void SimWorld::stepSimulation() {
    dynamicsWorld->stepSimulation(1/60.0);
}