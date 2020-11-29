//
// Created by rolf on 02-10-19.
//

#include <iostream>
#include "SimBall.h"
#include "CollisionShared.h"
SimBall::SimBall(std::shared_ptr<btMultiBodyDynamicsWorld> _world, const WorldSettings &settings,
        const btVector3 &initialPos,
        const btVector3 &initialVel)
        :
        world(_world),
        SCALE(settings.scale) {
    //create the shape and inertia
    physicsBall = new btSphereShape(settings.scale*settings.ballRadius);

    btVector3 inertia(1.0, 1.0, 1.0);
    inertia *= 0.4*settings.ballMass*settings.ballRadius*settings.ballRadius*SCALE*SCALE;

    multiBody = new btMultiBody(0, settings.ballMass, inertia, false, false, true);

  multiBody->setMaxCoordinateVelocity(100000000000);
    multiBody->setMaxAppliedImpulse(1000000000000);
    multiBody->setAngularDamping(0);
    multiBody->setLinearDamping(0);

    btTransform worldTransform;
    worldTransform.setIdentity();
    btVector3 startPos=initialPos;
    startPos.setZ(settings.ballRadius*SCALE);
    worldTransform.setOrigin(startPos);
    multiBody->setBaseWorldTransform(worldTransform);

    multiBody->finalizeMultiDof();

    world->addMultiBody(multiBody);

    motionState = new btDefaultMotionState(worldTransform); //TODO: use motionState?

    btMultiBodyLinkCollider* col = new btMultiBodyLinkCollider(multiBody, - 1);
    col->setCollisionShape(physicsBall);
    col->setWorldTransform(worldTransform);
    col->setUserPointer(this);
    world->addCollisionObject(col, COL_BALL, COL_GROUND | COL_ROBOT_HULL | COL_ROBOT_DRIBBLER | COL_BALL);
    // TODO: set restitution/friction
    //TODO: ang vel option+spinning friction and setting lin vel to 0
    col->setFriction(0.35);
    col->setRestitution(0.0);
    col->setRollingFriction(0.0357*7*0.2*settings.ballRadius*SCALE);
    col->setSpinningFriction(0.00);
    col->setCollisionFlags(col->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    col->setCcdMotionThreshold(1e-7);
    col->setCcdSweptSphereRadius(settings.ballRadius*SCALE);
    multiBody->setBaseCollider(col);

    multiBody->setBaseVel(initialVel);

}
SimBall::~SimBall() {
    world->removeMultiBody(multiBody);
    delete multiBody;
    delete physicsBall;
    delete motionState;
}

SimBall::SimBall(std::shared_ptr<btMultiBodyDynamicsWorld> _world, const WorldSettings &settings)
        :
        SimBall(_world, settings, btVector3(0.0f, 0.0f, settings.scale*settings.ballRadius)) {
}

btVector3 SimBall::velocity() const { //TODO: unused?
    return multiBody->getBaseVel();

}
btVector3 SimBall::position() const {
    const btTransform transform = multiBody->getBaseWorldTransform();
    return transform.getOrigin();
}
void SimBall::kick(const btVector3 &vel) {
    btVector3 velocity = vel;
    isKicked = true;
    kickedForce+=velocity;
}

btScalar SimBall::radius() const {
    return physicsBall->getRadius();
}

//Positions are added by the camera
SSL_DetectionBall SimBall::asDetection() const {
    SSL_DetectionBall ball;
    ball.set_area(0.0);//TODO: fix below 4 vars
    ball.set_confidence(1.0);
    //ball.set_z(physicsBall->getRadius()/SCALE);//TODO:  in replays we see this is not being set by SSL-vision Is this deprecated?

    return ball;
}
void SimBall::processKicks(double dt) {
  if(isKicked){
    isKicked = false;
    multiBody->addBaseForce(kickedForce/dt*multiBody->getBaseMass()*SCALE); //F*dt = m*v (impulse) , multiplied by the scale
    kickedForce = btVector3(0,0,0);
  }
}

void SimBall::instantKick(const btVector3 &velocity) {
    multiBody->setBaseVel(velocity);
}

void SimBall::dribble(const btVector3 &torque) {
    multiBody->addBaseTorque(torque);
}
