//
// Created by rolf on 19-09-19.
//

// The following is a great manual giving lots of tips hints and explanations on the library
// http://www.cs.kent.edu/~ruttan/GameEngines/lectures/Bullet_User_Manual
// this resource also has a lot of information:
// https://docs.google.com/document/d/10sXEhzFRSnvFcl3XxNGhnD4N2SedqwdAvK3dsihxVUA/edit#heading=h.2ye70wns7io3

#include "SimWorld.h"
#include "SimField.h"

SimWorld::SimWorld() {
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

    dynamicsWorld->setGravity(btVector3(0.0f,0.0f,-9.81f));
    ///TESTING
    btAlignedObjectArray<btCollisionShape*> collisionShapes;
    btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(6.3), btScalar(4.8), btScalar(0.5)));
    collisionShapes.push_back(groundShape);

    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(0, 0, 3));
    //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(0, myMotionState, groundShape, btVector3(0,0,0));
    btRigidBody* body = new btRigidBody(rbInfo);

    //add the body to the dynamics world
    dynamicsWorld->addRigidBody(body);
    field=new SimField(dynamicsWorld);
}
SimWorld::~SimWorld() {

    delete dynamicsWorld;
    delete solver;
    delete overlappingPairCache;
    delete collisionDispatcher;
    delete collisionConfig;
}
btDiscreteDynamicsWorld* SimWorld::getWorld() {
    return dynamicsWorld;
}