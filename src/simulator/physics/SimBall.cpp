//
// Created by rolf on 02-10-19.
//

#include "SimBall.h"

SimBall::SimBall(btDynamicsWorld *_world, WorldSettings *settings, const btVector3 &initialPos,
                 const btVector3 &initialVel) :
        world(_world) {
    //create the shape and inertia
    physicsBall = new btSphereShape(settings->ballRadius);
    btVector3 inertia(0.0f, 0.0f, 0.0f); // TODO check if inertia fits that of an actual sphere/the ball in our case
    physicsBall->calculateLocalInertia(settings->ballMass, inertia);

    //TODO: figure out how we can add initial velocity
    btTransform worldTransform;
    worldTransform.setIdentity();
    worldTransform.setOrigin(initialPos);
    motionState = new btDefaultMotionState(worldTransform);
    //construct the rigid body for collisions
    btRigidBody::btRigidBodyConstructionInfo rbInfo(settings->ballMass, motionState, physicsBall, inertia);
    body = new btRigidBody(rbInfo);
    // TODO: set restitution/friction
    body->setLinearVelocity(btVector3(0,0,0));//TODO test if this works and maybe add ang vel option
    body->setRestitution(1.0f);
    body->setFriction(1.0f);
    //add the constructed rigid Body to the world
    world->addRigidBody(body);
}
SimBall::~SimBall() {
    delete body;
    delete physicsBall;
    delete motionState;
}

SimBall::SimBall(btDynamicsWorld *_world, WorldSettings *settings) :
        SimBall(_world, settings, btVector3(0.0f, 0.0f, settings->ballRadius)) {
}

btVector3 SimBall::velocity() const {
    return body->getLinearVelocity();
}
btVector3 SimBall::position() const {
    const btTransform transform=body->getWorldTransform();
    return transform.getOrigin();
}