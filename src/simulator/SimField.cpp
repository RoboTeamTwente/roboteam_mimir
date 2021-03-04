//
// Created by rolf on 28-09-19.
//

#include "SimField.h"
#include "CollisionShared.h"

SimField::SimField(std::shared_ptr<btMultiBodyDynamicsWorld> world, const WorldSettings &cfg)
: dynamicsWorld(world)
{
    const float SCALE=cfg.scale;
    const float halfGoalHeight = cfg.goalHeight*0.5f*SCALE;
    const float halfGoalWidth = cfg.goalWidth*0.5f*SCALE;
    const float halfGoalDepth = cfg.goalDepth*0.5f*SCALE;
    const float halfGoalWallThickness = cfg.goalWallThickness*0.5f*SCALE;
    const float halfFieldLength = cfg.fieldLength*0.5f*SCALE;
    const float halfFieldWidth = cfg.fieldWidth*0.5f*SCALE;
    const float boundaryWidth =cfg.boundaryWidth*SCALE;
    const float ceilingHeight = cfg.ceilingHeight*SCALE;

    //create ground
    plane = new btStaticPlaneShape(btVector3(0.0, 0.0, 1.0),0);
    addObject(plane, btTransform(btQuaternion(btVector3(1, 0, 0), 0), btVector3(0, 0, 0)));
    //ceiling
    addObject(plane, btTransform(btQuaternion(btVector3(1, 0, 0), M_PI), btVector3(0, 0, ceilingHeight)));

    addObject(plane, btTransform(btQuaternion(btVector3(0.0, 1.0, 0.0), M_PI_2), btVector3(-(halfFieldLength + boundaryWidth), 0.0, 0.0)));
    addObject(plane, btTransform(btQuaternion(btVector3(0.0, 1.0, 0.0), -M_PI_2), btVector3(halfFieldLength + boundaryWidth, 0.0, 0.0)));

    addObject(plane, btTransform(btQuaternion(btVector3(1.0, 0.0, 0.0), -M_PI_2), btVector3(0.0f, -(halfFieldWidth + boundaryWidth), 0)));
    addObject(plane, btTransform(btQuaternion(btVector3(1.0, 0.0, 0.0), M_PI_2), btVector3(0.0f, halfFieldWidth + boundaryWidth, 0)));


    //Create back of goal box
    //the + is to fill the outer corners of the goal with an object too
    goalBack = new btBoxShape(goalBackDimensions(cfg));
    //Create goal Side
    goalSide = new btBoxShape(goalSideDimensions(cfg));

    auto backTransforms = goalBackTransforms(cfg);
    for(const auto& transform : backTransforms){
      addObject(goalBack,transform);
    }

    auto sideTransforms = goalSideTransforms(cfg);
    for(const auto& transform : sideTransforms){
      addObject(goalSide,transform);
    }
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
    object->setFriction(1.0);
//    object->setSpinningFriction(1.0);
    //allow custom friction for robot wheels, so we set the userIndex.
    object->setCollisionFlags(object->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);//TODO: only for bottom field

    int fieldCollidesWith = COL_BALL | COL_ROBOT_WHEEL | COL_ROBOT_HULL | COL_CAMERA_RAY;
    dynamicsWorld->addCollisionObject(object,COL_GROUND,fieldCollidesWith);
    objects.push_back(object);

}
std::vector<btTransform> SimField::goalBackTransforms(const WorldSettings &cfg) {
  const btQuaternion dir=btQuaternion::getIdentity();
  const float SCALE=cfg.scale;
  const float halfGoalHeight = cfg.goalHeight*0.5f*SCALE;
  const float halfGoalDepth = cfg.goalDepth*0.5f*SCALE;
  const float halfGoalWallThickness = cfg.goalWallThickness*0.5f*SCALE;
  const float halfFieldLength = cfg.fieldLength*0.5f*SCALE;
  const float halfLineWidth = cfg.lineWidth*0.5f*SCALE;

  return{
  btTransform(dir,btVector3(-(halfFieldLength+halfLineWidth+halfGoalWallThickness+halfGoalDepth*2),0.0f,halfGoalHeight)),
  btTransform(dir,btVector3(halfFieldLength+halfLineWidth+halfGoalWallThickness+halfGoalDepth*2,0.0f,halfGoalHeight))
  };
}
std::vector<btTransform> SimField::goalSideTransforms(const WorldSettings &cfg) {
  const btQuaternion dir=btQuaternion::getIdentity();
  const float SCALE=cfg.scale;
  const float halfGoalHeight = cfg.goalHeight*0.5f*SCALE;
  const float halfGoalWidth = cfg.goalWidth*0.5f*SCALE;
  const float halfGoalDepth = cfg.goalDepth*0.5f*SCALE;
  const float halfGoalWallThickness = cfg.goalWallThickness*0.5f*SCALE;
  const float halfFieldLength = cfg.fieldLength*0.5f*SCALE;
  const float halfLineWidth = cfg.lineWidth*0.5f*SCALE;

  return{
      btTransform(dir,btVector3(-(halfFieldLength+halfLineWidth+halfGoalDepth),-(halfGoalWidth+halfGoalWallThickness) ,halfGoalHeight)),
      btTransform(dir,btVector3(-(halfFieldLength+halfLineWidth+halfGoalDepth),halfGoalWidth+halfGoalWallThickness ,halfGoalHeight)),
      btTransform(dir,btVector3(halfFieldLength+halfLineWidth+halfGoalDepth,-(halfGoalWidth+halfGoalWallThickness) ,halfGoalHeight)),
      btTransform(dir,btVector3(halfFieldLength+halfLineWidth+halfGoalDepth,halfGoalWidth+halfGoalWallThickness ,halfGoalHeight))
      };
}
btVector3 SimField::goalBackDimensions(const WorldSettings &cfg) {
  const float SCALE=cfg.scale;
  const float halfGoalHeight = cfg.goalHeight*0.5f*SCALE;
  const float halfGoalWidth = cfg.goalWidth*0.5f*SCALE;
  const float halfGoalWallThickness = cfg.goalWallThickness*0.5f*SCALE;
  return btVector3(halfGoalWallThickness, halfGoalWidth + 2*halfGoalWallThickness, halfGoalHeight);
}
btVector3 SimField::goalSideDimensions(const WorldSettings &cfg) {
  const float SCALE=cfg.scale;
  const float halfGoalHeight = cfg.goalHeight*0.5f*SCALE;
  const float halfGoalDepth = cfg.goalDepth*0.5f*SCALE;
  const float halfGoalWallThickness = cfg.goalWallThickness*0.5f*SCALE;
  return btVector3(halfGoalDepth, halfGoalWallThickness, halfGoalHeight);
}