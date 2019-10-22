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

    worldTransform.setOrigin(btVector3(initialPos.x(),initialPos.y(),0.0));
    addWheels(settings, worldSettings,worldTransform);

}
void SimBot::addWheels(const RobotSettings *settings, const WorldSettings *worldSettings, btTransform hullTransform)  {
    //we don't want to construct the same shape 4 times
    btCylinderShapeX *wheelShape=new btCylinderShapeX(btVector3(settings->wheelThickness*0.5,settings->wheelRadius,settings->wheelRadius)*worldSettings->scale);
    addWheel(0,settings->wheelAngle0,wheelShape,settings,worldSettings,hullTransform);
    addWheel(1,settings->wheelAngle1,wheelShape,settings,worldSettings,hullTransform);
    addWheel(2,settings->wheelAngle2,wheelShape,settings,worldSettings,hullTransform);
    addWheel(3,settings->wheelAngle3,wheelShape,settings,worldSettings,hullTransform);

}
//TODO: make motors controllable
//TODO: fix initial offset of wheels at creation
void SimBot::addWheel(int wheelLabel, btScalar wheelAngleD, btCollisionShape *wheelShape, const RobotSettings *settings,
                      const WorldSettings *worldSettings,btTransform hullTransform) {
    btScalar angleR=wheelAngleD/180.0*M_PI;//convert to radians
    // find the centre of the wheel position
    btTransform wheelTransform;
    wheelTransform.setRotation(btQuaternion(btVector3(0.0f, 0.0f, 1.0f),angleR));
    btVector3 wheelPos=btVector3(cos(angleR)*settings->radius,sin(angleR)*settings->radius,settings->wheelRadius)*worldSettings->scale;
    wheelTransform.setOrigin(wheelPos);
    //calculate moments of inertia of wheel
    btVector3 wheelInertia;
    wheelShape->calculateLocalInertia(settings->wheelMass,wheelInertia);
    // cosntruct rigid body
    //we need to multiply by body transform to construct the objects in the right places
    btDefaultMotionState* motionState= new btDefaultMotionState(hullTransform*wheelTransform);
    btRigidBody::btRigidBodyConstructionInfo wheelInfo(settings->wheelMass,motionState,wheelShape,wheelInertia);
    btRigidBody * wheel= new btRigidBody(wheelInfo);
    //construct joint/motor
    btVector3 heightOffset=btVector3(0,0,-(settings->totalHeight*0.5+0.02))*worldSettings->scale; //TODO fix offsets and transforms (also wheel construction)
    btHingeConstraint * constraint = new btHingeConstraint(*body, *wheel,wheelPos+heightOffset,btVector3(0.0,0.0,0),btVector3(wheelPos.x(),wheelPos.y(),0),btVector3(1.0,0.0,0.0));
    constraint->enableAngularMotor(true,10,100);
    constraint->setDbgDrawSize(1);
    wheel->setFriction(0.8); //TODO: fix friction/rolling friction and ball collision
    //add everything to the world
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