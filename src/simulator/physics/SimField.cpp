//
// Created by rolf on 28-09-19.
//

#include "SimField.h"
#include "CollisionShared.h"

SimField::SimField(std::shared_ptr<btDynamicsWorld>world, std::shared_ptr<WorldSettings> cfg) {
    dynamicsWorld = world;
    const float SCALE=cfg->scale;
    const float halfGoalHeight = cfg->goalHeight*0.5f*SCALE;
    const float halfGoalWidth = cfg->goalWidth*0.5f*SCALE;
    const float halfGoalDepth = cfg->goalDepth*0.5f*SCALE;
    const float halfGoalWallThickness = cfg->goalWallThickness*0.5f*SCALE;
    const float halfFieldLength = cfg->fieldLength*0.5f*SCALE;
    const float halfFieldWidth = cfg->fieldWidth*0.5f*SCALE;
    const float halfLineWidth = cfg->lineWidth*0.5f*SCALE;
    const float boundaryWidth =cfg->boundaryWidth*SCALE;
    const float ceilingHeight = cfg->ceilingHeight*SCALE;

    //create ground
    plane = new btStaticPlaneShape(btVector3(0.0f, 0.0f, 1.0f), 0.0f);
    addObject(plane, btTransform(btQuaternion(btVector3(1, 0, 0), 0), btVector3(0, 0, 0)));
    //ceiling
    addObject(plane, btTransform(btQuaternion(btVector3(1, 0, 0), M_PI), btVector3(0, 0, ceilingHeight)));

    addObject(plane, btTransform(btQuaternion(btVector3(0.0f, 1.0f, 0.0f), M_PI_2), btVector3(-(halfFieldLength + boundaryWidth), 0.0f, 0.0f)));
    addObject(plane, btTransform(btQuaternion(btVector3(0.0f, 1.0f, 0.0f), -M_PI_2), btVector3(halfFieldLength + boundaryWidth, 0.0f, 0.0f)));

    addObject(plane, btTransform(btQuaternion(btVector3(1.0f, 0.0f, 0.0f), -M_PI_2), btVector3(0.0f, -(halfFieldWidth + boundaryWidth), 0)));
    addObject(plane, btTransform(btQuaternion(btVector3(1.0f, 0.0f, 0.0f), M_PI_2), btVector3(0.0f, halfFieldWidth + boundaryWidth, 0)));


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
    // when we exit from the field we need to destroy all relevant information stored in the dynamics world

    for (int i = 0; i < objects.size(); ++i) {
        dynamicsWorld->removeCollisionObject(objects[i]);
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
    //TODO: fix friction and restitution
    object->setRestitution(0.56);
    object->setFriction(1.0);
//    object->setSpinningFriction(1.0);
    //allow custom friction for robot wheels, so we set the userIndex.
    object->setUserIndex(bodyType::GROUND);//TODO: only set for the bottom field, not for all sides
    int fieldCollidesWith = COL_BALL | COL_ROBOT | COL_CAMERARAY;
    dynamicsWorld->addCollisionObject(object,COL_FIELD,fieldCollidesWith);
    objects.push_back(object);

}