//
// Created by rolf on 02-10-19.
//

#include <iostream>
#include "SimBall.h"
#include "CollisionShared.h"

SimBall::SimBall(std::shared_ptr<btDynamicsWorld> _world, std::shared_ptr<WorldSettings> settings, const btVector3 &initialPos,
                 const btVector3 &initialVel) :
        world(_world),
        SCALE(settings->scale)
        {
    //create the shape and inertia
    physicsBall = new btSphereShape(settings->scale*settings->ballRadius);
    btVector3 inertia(1.0,1.0,1.0);
    inertia*=0.4*settings->ballMass*settings->ballRadius*settings->ballRadius*SCALE*SCALE;
    btTransform worldTransform;
    worldTransform.setIdentity();
    worldTransform.setOrigin(initialPos);
    motionState = new btDefaultMotionState(worldTransform);
    //construct the rigid body for collisions
    btRigidBody::btRigidBodyConstructionInfo rbInfo(settings->ballMass, motionState, physicsBall, inertia);
    body = new btRigidBody(rbInfo);
    // TODO: set restitution/friction
    body->setLinearVelocity(initialVel);
    //TODO: ang vel option+spinning friction and setting lin vel to 0
    body->setRestitution(1.0f);
    body->setFriction(0.35f);
    // when rolling ball decelerates with friction force for linear as if it's 0.0357
    // However that does not take torque/angular rotation when rolling into account, so we need to multiply by
    // 7/5.0*ball radius from Torque computation.
    body->setRollingFriction(0.0357*7.0/5.0*SCALE*settings->ballRadius);
    body->setSpinningFriction(0.03);//TODO: test real values
    body->setSleepingThresholds(0.01*SCALE,0.01/2/M_PI);
    //add the constructed rigid Body to the world
    world->addRigidBody(body,COL_BALL,COL_FIELD|COL_ROBOT|COL_BALL);
}
SimBall::~SimBall() {
    delete body;
    delete physicsBall;
    delete motionState;
}

SimBall::SimBall(std::shared_ptr<btDynamicsWorld> _world, std::shared_ptr<WorldSettings> settings) :
        SimBall(_world, settings, btVector3(0.0f, 0.0f, settings->scale*settings->ballRadius)) {
}

btVector3 SimBall::velocity() const {
    return body->getLinearVelocity();
}
btVector3 SimBall::position() const {
    const btTransform transform=body->getWorldTransform();
    return transform.getOrigin();
}
void SimBall::kick(const btVector3 &force) {
    body->activate();
    body->applyCentralForce(force);
}

SSL_DetectionBall SimBall::asDetection() const {
    SSL_DetectionBall ball;
    const btVector3 &pos= position();
    ball.set_x(1000*pos.x()/SCALE);//TODO: check what SSL-Vision sends, rounded to integer?
    ball.set_y(1000*pos.y()/SCALE);
    ball.set_area(0.0);//TODO: fix below 4 vars
    ball.set_confidence(1.0);
    ball.set_pixel_x(0);
    ball.set_pixel_y(0);
    ball.set_z(physicsBall->getRadius()/SCALE);//TODO: is
    //TODO: figure out what's actually being sent by SSL-vision in this field and what units it has

    return ball;
}