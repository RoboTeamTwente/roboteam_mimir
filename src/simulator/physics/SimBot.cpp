//
// Created by rolf on 13-10-19.
//

#include "SimBot.h"

const float SCALE = 200;
btVector3 SimBot::position() const {
    btTransform transform;
    motionState->getWorldTransform(transform);
    return transform.getOrigin();
}

btScalar SimBot::orientation() const {
    btTransform transform;
    motionState->getWorldTransform(transform);
    return transform.getRotation().x();//TODO: actually test this function (is not working currently)
}
//TODO: add option of initializing with wheel velocities
SimBot::SimBot(btDynamicsWorld *world, RobotSettings *settings, const btVector3 &initialPos, btScalar dir) {
    dynamicsWorld = world;
    btCompoundShape *wholeShape = new btCompoundShape();
    btTransform shapeTransform;
    shapeTransform.setIdentity();

    //Create the outer hull of the robot
    RobotMesh mesh(settings);
    btConvexHullShape *convexHullShape = new btConvexHullShape();
    shapes.append(convexHullShape);
    for (btVector3 point : mesh.hull()) {
        //note scaling is done here so we do not need to worry about it in mesh construction
        convexHullShape->addPoint(point * SCALE);
    }
    wholeShape->addChildShape(shapeTransform, convexHullShape);
    shapes.append(wholeShape);

    //set the position of the hull
    btTransform worldTransform;
    worldTransform.setIdentity();
    btVector3 originPos(initialPos.x(), initialPos.y(), settings->height * 0.5);//TODO: ensure no offset!
    worldTransform.setOrigin(originPos);
    worldTransform.setRotation(btQuaternion(btVector3(0, 0, 1), dir));

    //make the hull a body with mass and place it into the world
    motionState = new btDefaultMotionState(worldTransform);
    btVector3 localInertia(0, 0, 0);
    wholeShape->calculateLocalInertia(settings->bodyMass, localInertia);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(settings->bodyMass, motionState, wholeShape, localInertia);
    body = new btRigidBody(rbInfo);
    body->setFriction(1.0);
    body->setRestitution(0.0);
    dynamicsWorld->addRigidBody(body);
}
SimBot::SimBot(btDynamicsWorld *world, RobotSettings *settings) : SimBot(world, settings,
                                                                         btVector3(0, 0, settings->height * 0.5), 0.0) {
}
SimBot::~SimBot() {
    dynamicsWorld->removeRigidBody(body);
    delete body;
    delete motionState;
    while (!shapes.empty()) {
        delete shapes.last();
        shapes.removeLast();
    }


}