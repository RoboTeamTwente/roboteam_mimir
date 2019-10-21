//
// Created by rolf on 13-10-19.
//

#include "SimBot.h"

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
SimBot::SimBot(btDynamicsWorld *world, RobotSettings *settings, WorldSettings *worldSettings, const btVector3 &initialPos, btScalar dir) {
    dynamicsWorld = world;
    btCompoundShape *wholeShape = new btCompoundShape();
    btTransform shapeTransform;
    shapeTransform.setIdentity();

    //Create the outer hull of the robot
    RobotMesh mesh(settings);
    btConvexHullShape *convexHullShape = new btConvexHullShape();
    shapes.push_back(convexHullShape);
    for (btVector3 point : mesh.hull()) {
        //note scaling is done here so we do not need to worry about it in mesh construction
        convexHullShape->addPoint(point * worldSettings->scale);
    }
    wholeShape->addChildShape(shapeTransform, convexHullShape);
    shapes.push_back(wholeShape);

    //set the position of the hull
    btTransform worldTransform;
    worldTransform.setIdentity();
    btVector3 originPos(initialPos.x(), initialPos.y(), worldSettings->scale*((settings->totalHeight - settings->bottomPlateHeight)* 0.5+settings->bottomPlateHeight));//TODO: ensure no offset!
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
    addWheel(settings, worldSettings);

}
void SimBot::addWheel(const RobotSettings *settings, const WorldSettings *worldSettings) const {//create wheels
    btCylinderShapeX *wheelShape=new btCylinderShapeX(btVector3(settings->wheelThickness*0.5,settings->wheelRadius,settings->wheelRadius)*worldSettings->scale);
    btTransform wheelTransform;
    wheelTransform.setIdentity();
    btScalar angle=settings->wheelAngle0/180.0*M_PI;
    wheelTransform.setRotation(btQuaternion(btVector3(0.0f, 0.0f, 1.0f),angle));
    btVector3 wheelPos=btVector3(cos(angle)*settings->radius,sin(angle)*settings->radius,settings->wheelRadius)*worldSettings->scale;
    wheelTransform.setOrigin(wheelPos);

    btDefaultMotionState *motionState1=new btDefaultMotionState(wheelTransform);
    btVector3 wheelInertia;
    wheelShape->calculateLocalInertia(settings->wheelMass,wheelInertia);
    btRigidBody::btRigidBodyConstructionInfo wheelInfo(settings->wheelMass,motionState1,wheelShape,wheelInertia);
    btRigidBody * wheel= new btRigidBody(wheelInfo);
    btVector3 heightOffset=btVector3(0,0,-(settings->totalHeight*0.5))*worldSettings->scale;
    btHingeConstraint * constraint = new btHingeConstraint(*body, *wheel,wheelPos+heightOffset,btVector3(0.0,0.0,0),btVector3(wheelPos.x(),wheelPos.y(),0),btVector3(1.0,0.0,0.0));
    constraint->enableAngularMotor(true,10,100);
    constraint->setDbgDrawSize(100);

    dynamicsWorld->addConstraint(constraint, true);
    dynamicsWorld->addRigidBody(wheel);

}
SimBot::SimBot(btDynamicsWorld *world, RobotSettings *settings, WorldSettings * worldSettings) : SimBot(world, settings,worldSettings,
                                                                         btVector3(0, 0, 0), 0.0) {
}
SimBot::~SimBot() {
    dynamicsWorld->removeRigidBody(body);
    delete body;
    delete motionState;
    for (int i = shapes.size(); i >=0; --i) {
        delete shapes[i];
    }
}