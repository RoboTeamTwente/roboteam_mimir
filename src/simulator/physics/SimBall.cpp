//
// Created by rolf on 02-10-19.
//

#include <iostream>
#include <iomanip>
#include "SimBall.h"
SimBall::SimBall(btDynamicsWorld *_world, WorldSettings *settings, const btVector3 &initialPos,
                 const btVector3 &initialVel) :
        world(_world) {
    //create the shape and inertia
    physicsBall = new btSphereShape(settings->scale*settings->ballRadius);
    btVector3 inertia(1.0,1.0,1.0);
    inertia*=0.4*settings->ballMass*settings->ballRadius*settings->ballRadius*settings->scale*settings->scale;
    //TODO: figure out how we can add initial velocity
    btTransform worldTransform;
    worldTransform.setIdentity();
    worldTransform.setOrigin(initialPos);
    motionState = new btDefaultMotionState(worldTransform);
    //construct the rigid body for collisions
    btRigidBody::btRigidBodyConstructionInfo rbInfo(settings->ballMass, motionState, physicsBall, inertia);
    body = new btRigidBody(rbInfo);
    // TODO: set restitution/friction
    body->setLinearVelocity(initialVel);//TODO test if this works and maybe add ang vel option
    body->setRestitution(1.0f);
    body->setFriction(1.0f);
    body->setRollingFriction(0.0357f);//TODO: fix rolling friction/two stage ball model/scaling problems (Possibly caused by inertia?)
    std::cout<<body->getAngularSleepingThreshold()<<std::endl;
    body->setSleepingThresholds(0.01*settings->scale,0.01/2/M_PI);
    //add the constructed rigid Body to the world
    world->addRigidBody(body);
}
SimBall::~SimBall() {
    delete body;
    delete physicsBall;
    delete motionState;
}

SimBall::SimBall(btDynamicsWorld *_world, WorldSettings *settings) :
        SimBall(_world, settings, btVector3(0.0f, 0.0f, settings->scale*settings->ballRadius)) {
}

btVector3 SimBall::velocity() const {
    return body->getLinearVelocity();
}
void print(btVector3 data){
    std::cout<<data.x()<<" "<<data.y()<<" "<<data.z()<<std::endl;
}
btVector3 SimBall::position() {
    const btTransform transform=body->getWorldTransform();
    //btVector3 linVel=body->getLinearVelocity();
    //btVector3 angVel=body->getAngularVelocity();
    //std::cout<<"_______"<<std::endl;
//    print(linVel/100);
//    print(angVel);
    //float vel=linVel.norm();
    //std::cout<<std::setprecision(10)<<vel/100.0<<std::endl;
    //std::cout<<tick<<std::endl;
    tick++;
    std::cout<<"_______"<<std::endl;
    return transform.getOrigin();
}
