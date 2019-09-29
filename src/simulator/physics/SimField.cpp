//
// Created by rolf on 28-09-19.
//

#include "SimField.h"

SimField::SimField(btDynamicsWorld *world) {
    dynamicsWorld = world;

    //measurements (in meters) as by the SSL rulebook as of 29 september 2019
    const float halfGoalHeight = 0.08;
    const float halfGoalWidth = 0.6;
    const float halfGoalDepth = 0.09;
    const float halfGoalWallThickness = 0.01;

    const float halfFieldLength = 6.0;
    const float halfFieldWidth = 4.5;
    const float halfLineWidth = 0.005;
    const float boundaryWidth = 0.3;
    const float ceilingHeight = 5.0;

    //create ground
    plane = new btStaticPlaneShape(btVector3(0.0f, 0.0f, 1.0f), 0.0f);
    addObject(plane, btTransform(btQuaternion(btVector3(1, 0, 0), 0), btVector3(0, 0, 0)));
    //ceiling
    addObject(plane, btTransform(btQuaternion(btVector3(1, 0, 0), M_PI), btVector3(0, 0, ceilingHeight)));

    addObject(plane, btTransform(btQuaternion(btVector3(0.0f, 1.0f, 0.0f), M_PI_2), btVector3(-(halfFieldLength + boundaryWidth), 0.0f, 0.0f)));
    addObject(plane, btTransform(btQuaternion(btVector3(0.0f, 1.0f, 0.0f), -M_PI_2), btVector3(halfFieldLength + boundaryWidth, 0.0f, 0.0f)));

    addObject(plane, btTransform(btQuaternion(btVector3(1.0f, 0.0f, 0.0f), M_PI_2), btVector3(0.0f, -(halfFieldWidth + boundaryWidth), 0)));
    addObject(plane, btTransform(btQuaternion(btVector3(1.0f, 0.0f, 0.0f), -M_PI_2), btVector3(0.0f, halfFieldWidth + boundaryWidth, 0)));


    //Create back of goal box
    //the + is to fill the outer corners of the goal with an object too
    goalBack = new btBoxShape(btVector3(halfGoalWallThickness, halfGoalWidth + 2*halfGoalWallThickness, halfGoalHeight));
    //Create goal Side
    goalSide = new btBoxShape(btVector3(halfGoalDepth, halfGoalWallThickness, halfGoalHeight));
    //Let's put the goals in the correct places
    const btQuaternion dir=btQuaternion::getIdentity();

    //goal on negative x side
    addObject(goalBack,btTransform(dir,btVector3(-(halfFieldLength+halfLineWidth+halfGoalWallThickness+halfGoalDepth*2),0.0f,halfGoalHeight)));
    addObject(goalSide,btTransform(dir,btVector3(-(halfFieldLength+halfLineWidth+halfGoalDepth),-(halfGoalWidth+halfGoalWallThickness) ,halfGoalHeight)));
    addObject(goalSide,btTransform(dir,btVector3(-(halfFieldLength+halfLineWidth+halfGoalDepth),halfGoalWidth+halfGoalWallThickness ,halfGoalHeight)));

    //goal on positive x side
    addObject(goalBack,btTransform(dir,btVector3(halfFieldLength+halfLineWidth+halfGoalWallThickness+halfGoalDepth*2,0.0f,halfGoalHeight)));
    addObject(goalSide,btTransform(dir,btVector3(halfFieldLength+halfLineWidth+halfGoalDepth,-(halfGoalWidth+halfGoalWallThickness) ,halfGoalHeight)));
    addObject(goalSide,btTransform(dir,btVector3(halfFieldLength+halfLineWidth+halfGoalDepth,halfGoalWidth+halfGoalWallThickness ,halfGoalHeight)));
}
SimField::~SimField() {
    // when we exit from the field we need to destory all relevant information stored in the dynamics world
    for (int i = 0; i < objects.size(); ++i) {
        delete objects[i];
    }
    delete goalSide;
    delete goalBack;
    delete plane;
}
void SimField::addObject(btCollisionShape *shape, const btTransform &transform) {
    //create a new object for collisions
    btCollisionObject *object = new btCollisionObject;
    object->setCollisionShape(shape);
    // put it into the world
    object->setWorldTransform(transform);

    object->setRestitution(0.0);
    object->setFriction(0.0);
    object->setRollingFriction(0.0);
    dynamicsWorld->addCollisionObject(object);
    objects.push_back(object);

}