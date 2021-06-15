//
// Created by rolf on 13-10-19.
//

#include "SimBot.h"

#include "iostream"
#include "CollisionShared.h"
#include "SimBall.h"

btVector3 SimBot::position() const {
    btTransform transform;
    motionState->getWorldTransform(transform);
    btVector3 origin=transform.getOrigin();
    transform.setOrigin(btVector3(0,0,0));
    btVector3 topPos= origin+transform*(btVector3(0,0,robSettings.totalHeight-robSettings.bottomPlateHeight)*0.5*SCALE);
    return topPos;
}

btScalar SimBot::orientation() const {
    btTransform transform;
    motionState->getWorldTransform(transform);
    btScalar yaw, pitch, roll;
    transform.getRotation().getEulerZYX(yaw, pitch, roll);
    return yaw;
}
//TODO: add option of initializing with wheel velocities
SimBot::SimBot(unsigned int _id, std::shared_ptr<btMultiBodyDynamicsWorld> world, const RobotSettings &settings,
               const WorldSettings& worldSettings, const btVector3 &initialPos, btScalar dir) :
        id{_id},
        SCALE(worldSettings.scale),
        robSettings{settings},
        dynamicsWorld{world}
{
    btCompoundShape *wholeShape = new btCompoundShape();
    btTransform shapeTransform;
    shapeTransform.setIdentity();
    //Create the outer hull of the robot
    RobotMesh mesh(robSettings);
    btConvexHullShape *convexHullShape = new btConvexHullShape();
    shapes.push_back(convexHullShape);
    for (btVector3 point : mesh.complete_hull()) {
        //note scaling is done here so we do not need to worry about it in mesh construction
        convexHullShape->addPoint(point * SCALE);
    }
    wholeShape->addChildShape(shapeTransform, convexHullShape);
    shapes.push_back(wholeShape);

    //set the position of the hull
    btTransform worldTransform;
    worldTransform.setIdentity();
    btVector3 originPos(initialPos.x(), initialPos.y(), SCALE *
                                                        ((settings.totalHeight - settings.bottomPlateHeight) * 0.5 +
                                                         settings.bottomPlateHeight));//TODO: ensure no offset!
    worldTransform.setOrigin(originPos);
    worldTransform.setRotation(btQuaternion(btVector3(0, 0, 1), dir));

    //make the hull a body with mass and place it into the world
    motionState = new btDefaultMotionState(worldTransform);
    btVector3 localInertia(0, 0, 0);
    wholeShape->calculateLocalInertia(settings.bodyMass, localInertia);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(settings.bodyMass, motionState, wholeShape, localInertia);
    body = new btRigidBody(rbInfo);
    body->setFriction(1.0);
    body->setRestitution(0.0);
    body->setDeactivationTime(1.0);

  dynamicsWorld->addRigidBody(body,COL_ROBOT_HULL,COL_EVERYTHING);
    body->setCollisionFlags(btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    worldTransform.setOrigin(btVector3(initialPos.x(), initialPos.y(), 0.0));
    addWheels(worldSettings, worldTransform);
    front_end = new SimBotFrontEnd(world,worldSettings,robSettings,shapeTransform,body,this);
}
void
SimBot::addWheels(const WorldSettings& worldSettings,
                  btTransform hullTransform) {
    //we don't want to construct the same shape 4 times
    btCylinderShapeX *wheelShape = new btCylinderShapeX(
            btVector3(robSettings.wheelThickness * 0.5, robSettings.wheelRadius, robSettings.wheelRadius) *
            SCALE);
    addWheel(0, robSettings.wheelAngle0, wheelShape,  worldSettings, hullTransform);
    addWheel(1, robSettings.wheelAngle1, wheelShape,  worldSettings, hullTransform);
    addWheel(2, robSettings.wheelAngle2, wheelShape,  worldSettings, hullTransform);
    addWheel(3, robSettings.wheelAngle3, wheelShape,  worldSettings, hullTransform);
}
void SimBot::addWheel(int wheelLabel, btScalar wheelAngleD, btCollisionShape *wheelShape,
                      const WorldSettings& worldSettings, btTransform hullTransform) {
    btScalar angleR = wheelAngleD / 180.0 * M_PI;//convert to radians
    // find the centre of the wheel position
    btTransform wheelTransform;
    wheelTransform.setRotation(btQuaternion(btVector3(0.0f, 0.0f, 1.0f), angleR));
    btVector3 wheelPos =
            btVector3(cos(angleR) * robSettings.radius, sin(angleR) * robSettings.radius, robSettings.wheelRadius) *
            SCALE;
    wheelTransform.setOrigin(wheelPos);
    //calculate moments of inertia of wheel
    btVector3 wheelInertia;
    wheelShape->calculateLocalInertia(robSettings.wheelMass, wheelInertia);
    // cosntruct rigid body
    //we need to multiply by body transform to construct the objects in the right places
    btDefaultMotionState *motionState = new btDefaultMotionState(hullTransform * wheelTransform);
    btRigidBody::btRigidBodyConstructionInfo wheelInfo(robSettings.wheelMass, motionState, wheelShape, wheelInertia);
    btRigidBody *wheel = new btRigidBody(wheelInfo);
    wheels[wheelLabel] = wheel;
    //construct joint/motor
    btVector3 heightOffset = btVector3(0, 0, -(robSettings.totalHeight+robSettings.bottomPlateHeight) * 0.5) *
                             SCALE;
    btHingeConstraint *constraint = new btHingeConstraint(*body, *wheel, wheelPos + heightOffset,
                                                          btVector3(0.0, 0.0, 0),
                                                          btVector3(wheelPos.x(), wheelPos.y(), 0),
                                                          btVector3(1.0, 0.0, 0.0));
    constraint->enableAngularMotor(true, 0, 10000000);
    constraint->setDbgDrawSize(1.5);
    wheel->setCollisionFlags(wheel->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    // set friction to be different in each direction of the wheel axis x is perpendicular, y is tangent. Z friction is lateral (only really relevant for ball/ball and robot/robot collisions)
    wheel->setAnisotropicFriction(btVector3(0.05, 1.0, 0.0), btCollisionObject::CF_ANISOTROPIC_FRICTION);
    wheel->setFriction(1.5);
    wheelMotor[wheelLabel] = constraint;
    //add everything to the world
    dynamicsWorld->addConstraint(constraint, true);
    dynamicsWorld->addRigidBody(wheel,COL_ROBOT_WHEEL,COL_EVERYTHING);

}
void SimBot::wheelControl(btScalar wheel0, btScalar wheel1, btScalar wheel2, btScalar wheel3) {
    wheelMotor[0]->setMotorTargetVelocity(wheel0);
    wheelMotor[1]->setMotorTargetVelocity(wheel1);
    wheelMotor[2]->setMotorTargetVelocity(wheel2);
    wheelMotor[3]->setMotorTargetVelocity(wheel3); //TODO: make 4 a constant called NWHEEL or so
}
void SimBot::localControl(btScalar velTangent, btScalar velNormal, btScalar velAngle) {
    btScalar degToRad = M_PI / 180.0f;
    btScalar angles[4] = {degToRad * robSettings.wheelAngle0,
                          degToRad * robSettings.wheelAngle1,
                          degToRad * robSettings.wheelAngle2,
                          degToRad * robSettings.wheelAngle3};
    for (int i = 0; i < 4; ++i) {
        btScalar wheelVel = 1.0 / robSettings.wheelRadius *
                            ((robSettings.radius * velAngle) - (velTangent * sin(angles[i])) +
                             (velNormal * cos(angles[i])));
        wheelMotor[i]->setMotorTargetVelocity(wheelVel);
    }
}
SimBot::~SimBot() {
    delete front_end;
    for (int i = shapes.size() - 1; i >= 0; --i) {
        delete shapes[i];
    }
    for (int j = 0; j < 4; ++j) {
        dynamicsWorld->removeRigidBody(wheels[j]);
        dynamicsWorld->removeConstraint(wheelMotor[j]);
        delete wheels[j];
        delete wheelMotor[j];
    }
    dynamicsWorld->removeRigidBody(body);
    delete body;
    delete motionState;

}
unsigned int SimBot::getId() const {
    return id;
}

//TODO: move to base class.
void SimBot::receiveCommand(const mimir_robotcommand &robotcommand, double time) {
    //We don't apply the commands yet, we just receive here.
    if (robotcommand.id() != id) {
        std::cerr << "Sending command to wrong robot!" << std::endl;
        return;
    }
    lastCommandTime = time; //TODO: fix timing for commands (the layer above this) so that in between time is also fixed.
    isActive = true;
    lastCommand = robotcommand;
}
void SimBot::globalControl(btScalar xVel, btScalar yVel, btScalar angularVel) {
    btScalar robotAngle = orientation(); //We assume the robot knows it's absolute rotation
    //clockwise rotation since we rotate back to robot frame
    localControl(xVel * cos(robotAngle) + yVel * sin(robotAngle), -xVel * sin(robotAngle) + yVel * cos(robotAngle),
                 angularVel);
}
void printVector(const btVector3& vec){
    std::cout<<vec.x()<<" "<<vec.y()<<" "<<vec.z()<<std::endl;
}
//Simulate the internal code loop on the robot.
void SimBot::update(SimBall *ball, double time) {
    //The real robot stops after not receiving commands for some time
    if (time - lastCommandTime > 0.1) { //TODO: move to settings
        deactivate();
        return;
    }
    body->activate();
    switch (lastCommand.control_case()) {
        case mimir_robotcommand::kWheels: {
            const auto &w = lastCommand.wheels();
            wheelControl(w.wheel1(), w.wheel2(), w.wheel3(), w.wheel4());
            break;
        }
        case mimir_robotcommand::kGlobalVel: {
            if (lastCommand.globalvel().angleControl_case() == lastCommand.globalvel().kAngle) {
                const auto &g = lastCommand.globalvel();
                globalControlAngle(g.velx(),g.vely(),g.angle(),time-lastCommandTime);
            } else {
                const auto &g = lastCommand.globalvel();
                globalControl(g.velx(), g.vely(), g.anglevel());
            }
            break;
        }
        case mimir_robotcommand::kRobotVel: {
            const auto &r = lastCommand.robotvel();
            localControl(r.veltangent(), r.velnormal(), r.velangle());
            break;
        }
        case mimir_robotcommand::CONTROL_NOT_SET: {
            //std::cerr << "No control set in command!" << std::endl;
            localControl(-0.02,0.0,0.0);
            break; //TODO: fix this error on startup
        }
    }


}
void SimBot::deactivate() {
    if (isActive) {
        wheelControl(0, 0, 0, 0);
        isActive = false;
    }
}

bool SimBot::canKickBall(SimBall *ball) {
    //We say that the robot can kick the ball if it within 3 cm of it's front end
    double distance = 0.03*SCALE;
    double ballHeight = ball->position().z();
    //The ball cannot be flying
    if(ballHeight>0.05*SCALE + ball->radius()){
        return false;
    }
    btScalar yaw,pitch,roll;
    body->getWorldTransform().getRotation().getEulerZYX(yaw,pitch,roll);
    double startAngle = robSettings.startAngle/180.0*M_PI;
    btVector3 robotFrontCenter = body->getWorldTransform() * (btVector3(robSettings.radius*cos(startAngle),0,0)*SCALE);
    btVector3 robotFrontNormal = (robotFrontCenter-body->getWorldTransform().getOrigin()).normalized();

    btVector3 ballPos = ball->position();
    btVector3 v = ballPos - robotFrontCenter;
    btScalar dist = v.dot(robotFrontNormal);
    btVector3 projection = ballPos - dist*robotFrontNormal;
    btVector3 diff = projection-robotFrontCenter;

    double xydist = sqrt(diff.x()*diff.x()+diff.y()*diff.y());

    double endAngle = robSettings.endAngle/180.0*M_PI;
    double totalCutAngle = M_PI-(endAngle-startAngle)*0.5;
    double halfFrontLength = sin(totalCutAngle)*robSettings.radius*SCALE;

    btVector3 projDiff = ballPos-projection;
    double normalDist = sqrt(projDiff.x()*projDiff.x()+projDiff.y()*projDiff.y());
    return xydist < halfFrontLength && normalDist < SCALE*0.05;
}


//Position/orientation info is done on a higher level in combination with the camera info.
SSL_DetectionRobot SimBot::asDetection() const {
    SSL_DetectionRobot robot;
    robot.set_height(height());
    robot.set_robot_id(id);
    robot.set_confidence(1.0);
    return robot;
}
btScalar SimBot::height() const {
    return robSettings.totalHeight;
}
void SimBot::globalControlAngle(btScalar xVel, btScalar yVel, btScalar targetAngle, btScalar dt) {
    btScalar robotAngle = constrainAngle(orientation()); //We assume the robot knows it's absolute rotation
    //clockwise rotation since we rotate back to robot frame
    btScalar velTangent=xVel * cos(robotAngle) + yVel * sin(robotAngle);
    btScalar velNormal =-xVel * sin(robotAngle) + yVel * cos(robotAngle);
    // compute difference (P gain)
    btScalar deltaAngle= constrainAngle(targetAngle-robotAngle);
    // compute derivative estimate (D gain)
    btScalar angularVel=(robotAngle-lastYaw)*250;
    lastYaw = robotAngle;
    //Simple angle PID controller.
    btScalar P = 12.0;
    btScalar D = 2.0;
    btScalar pidP = P*deltaAngle;
    btScalar pidD = -angularVel*D;
    localControl(velTangent,velNormal,pidP+pidD);
}

btScalar SimBot::constrainAngle(btScalar angle){
    angle = fmod(angle + M_PI, 2*M_PI);
    if (angle < 0){
        angle += 2*M_PI;
    }
    angle-=M_PI;
    //extra insurance
    if (angle< - M_PI){
        angle += 2*M_PI;
    }
    else if (angle > M_PI){
        angle -= 2*M_PI;
    }
    return angle;
}
btTransform SimBot::kickerWorldTransform() {
  return front_end->frontEndBody->getWorldTransform();
}
mimir_robotcommand SimBot::current_command() const {
  return lastCommand;
}
bool SimBot::is_active() const {
  return isActive;
}
SimBotFrontEnd::SimBotFrontEnd(std::shared_ptr<btMultiBodyDynamicsWorld> world,const WorldSettings &worldSettings, const RobotSettings &settings,
                               btTransform robotHullTransform, btRigidBody * robotBody, SimBot * robot) :
dynamicsWorld(world),
SCALE(worldSettings.scale){
  printVector(robotBody->getWorldTransform().getOrigin());
  btScalar z,y,x;
  robotBody->getWorldTransform().getRotation().getEulerZYX(z,y,x);
  boxShape = new btBoxShape(
      btVector3(settings.kickerThickness, settings.kickerWidth, settings.kickerHeight) * SCALE);  //TODO: put constants in settings
  btVector3 dribblerCenter =
      btVector3(settings.radius - 0.015, 0, -0.04) * SCALE;


  btTransform dribblerStartTransform;
  dribblerStartTransform.setIdentity();
  dribblerStartTransform.setOrigin(dribblerCenter);
  dribblerStartTransform = robotBody->getWorldTransform()  * robotHullTransform*  dribblerStartTransform;
  btVector3 dribblerInertia(0, 0, 0);

  boxShape->calculateLocalInertia(0.02 * settings.bodyMass, dribblerInertia); //TODO: put into settings
  btRigidBody::btRigidBodyConstructionInfo rbDribInfo(0.02 * settings.bodyMass, nullptr, boxShape,
                                                      dribblerInertia);
  rbDribInfo.m_startWorldTransform = dribblerStartTransform;

  frontEndBody = new btRigidBody(rbDribInfo);
  frontEndBody->setUserPointer(this);
  frontEndBody->setFriction(0.0);


    dynamicsWorld->addRigidBody(frontEndBody, COL_ROBOT_DRIBBLER, COL_BALL);

  btTransform localA, localB;
  localA.setIdentity();
  localB.setIdentity();
  localA.setOrigin(dribblerCenter);

  robotConstraint = new btFixedConstraint(*robotBody,*frontEndBody,localA,localB);

  dynamicsWorld->addConstraint(robotConstraint, true);

}
bool SimBotFrontEnd::ballCollisionCallback(SimBall * ball, btManifoldPoint& contactPoint) {
//TODO: also add reactionary forces onto robot?

 mimir_robotcommand command = robot->current_command();
 btVector3 collisionNormal = contactPoint.m_normalWorldOnB;

 //todo add charging
 if(command.has_kicker() && command.kicker().kickchippower() > 0.0 ){
   if(!command.kicker().chip()){
     btScalar velocity = command.kicker().kickchippower();
     btVector3 vec = collisionNormal.normalized()*velocity;
     ball->kick(vec);
     return false;
   }else{
     btScalar kickVelocity = command.kicker().kickchippower();
     btScalar angle = 1.2*M_PI_4; //TODO: make robot setting or a model
     btScalar upVelLength = collisionNormal.length()*tan(angle);
     btVector3 kickDir = collisionNormal + getLocalUp().normalized()*upVelLength;
     btVector3 kickVec = kickDir.normalized() * kickVelocity;
     ball->kick(kickVec);
     return false;
   }
 }

  //dribbling: add friction and simulate that the surface is moving down at a velocity
//  std::cout<<contactPoint.m_contactMotion1<<std::endl;
//  //btVector3 torqueVec = contactPoint.m_localPointA.cross();
//  ball->dribble(getLocalUp().normalized()*2);
  return false;
}

btVector3 SimBotFrontEnd::getLocalUp() const{
    return frontEndBody->getWorldTransform().getBasis().getColumn(2);
}

SimBotFrontEnd::~SimBotFrontEnd() {
    dynamicsWorld->removeRigidBody(frontEndBody);
    dynamicsWorld->removeConstraint(robotConstraint);
    delete frontEndBody;
    delete robotConstraint;
}
