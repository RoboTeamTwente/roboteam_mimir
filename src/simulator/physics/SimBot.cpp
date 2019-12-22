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
    return transform.getOrigin();
}

btScalar SimBot::orientation() const {
    btTransform transform;
    motionState->getWorldTransform(transform);
    btScalar yaw, pitch, roll;
    transform.getRotation().getEulerZYX(yaw, pitch, roll);
    return yaw;
}
//TODO: add option of initializing with wheel velocities
SimBot::SimBot(unsigned int _id, std::unique_ptr<btDiscreteDynamicsWorld>& world, const std::unique_ptr<RobotSettings> &settings,
               const std::unique_ptr<WorldSettings>& worldSettings, const btVector3 &initialPos, btScalar dir) :
        id{_id},
        SCALE(worldSettings->scale),
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
    for (btVector3 point : mesh.hull()) {
        //note scaling is done here so we do not need to worry about it in mesh construction
        convexHullShape->addPoint(point * SCALE);
    }
    wholeShape->addChildShape(shapeTransform, convexHullShape);
    shapes.push_back(wholeShape);

    //set the position of the hull
    btTransform worldTransform;
    worldTransform.setIdentity();
    btVector3 originPos(initialPos.x(), initialPos.y(), SCALE *
                                                        ((settings->totalHeight - settings->bottomPlateHeight) * 0.5 +
                                                         settings->bottomPlateHeight));//TODO: ensure no offset!
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
    dynamicsWorld->addRigidBody(body,COL_ROBOT,COL_EVERYTHING);

    worldTransform.setOrigin(btVector3(initialPos.x(), initialPos.y(), 0.0));
    addWheels(worldSettings, worldTransform);
    addDribbler( worldSettings, dir, originPos);
}
void SimBot::addDribbler(const std::unique_ptr<WorldSettings> &worldSettings,
                    btScalar dir, const btVector3 &originPos) {
    //TODO: fix dimensions and offsets to be accurate
//TODO: put constants in settings
    btCylinderShape *dribblerShape = new btCylinderShapeX(
            btVector3(0.1 / 2.0f, 0.007f, 0.007f) * SCALE);
    shapes.push_back(dribblerShape);
    btVector3 dribblerCenter =
            btVector3(robSettings->radius - 0.015, 0, -robSettings->totalHeight / 2.0f + 0.03f) * SCALE;
    btTransform dribblerStartTransform;
    dribblerStartTransform.setIdentity();
    dribblerStartTransform.setOrigin(dribblerCenter + originPos);
    dribblerStartTransform.setRotation(btQuaternion(btVector3(0, 0, 1), dir + M_PI_2));

    btVector3 dribblerInertia(0, 0, 0);
    dribblerShape->calculateLocalInertia(0.02 * robSettings->bodyMass, dribblerInertia);
    btRigidBody::btRigidBodyConstructionInfo rbDribInfo(0.02 * robSettings->bodyMass, nullptr, dribblerShape,
                                                        dribblerInertia);
    rbDribInfo.m_startWorldTransform = dribblerStartTransform;

    dribbler = new btRigidBody(rbDribInfo);
    dribbler->setRestitution(0.2f);
    dribbler->setFriction(3.5f);
    dynamicsWorld->addRigidBody(dribbler, COL_ROBOT, COL_EVERYTHING);

    btTransform localA, localB;
    localA.setIdentity();
    localB.setIdentity();
    localA.setOrigin(dribblerCenter);
    localA.setRotation(btQuaternion(btVector3(1, 0, 0), - M_PI_2));
    localB.setRotation(btQuaternion(btVector3(0, 1, 0), M_PI_2));
    dribblerMotor = new btHingeConstraint(*body, *dribbler, localA, localB);
    dribblerMotor->enableAngularMotor(true,0, 1000);
    dynamicsWorld->addConstraint(dribblerMotor, true);
}
void
SimBot::addWheels(const std::unique_ptr<WorldSettings>& worldSettings,
                  btTransform hullTransform) {
    //we don't want to construct the same shape 4 times
    btCylinderShapeX *wheelShape = new btCylinderShapeX(
            btVector3(robSettings->wheelThickness * 0.5, robSettings->wheelRadius, robSettings->wheelRadius) *
            SCALE);
    addWheel(0, robSettings->wheelAngle0, wheelShape,  worldSettings, hullTransform);
    addWheel(1, robSettings->wheelAngle1, wheelShape,  worldSettings, hullTransform);
    addWheel(2, robSettings->wheelAngle2, wheelShape,  worldSettings, hullTransform);
    addWheel(3, robSettings->wheelAngle3, wheelShape,  worldSettings, hullTransform);
}
void SimBot::addWheel(int wheelLabel, btScalar wheelAngleD, btCollisionShape *wheelShape,
                      const std::unique_ptr<WorldSettings>& worldSettings, btTransform hullTransform) {
    btScalar angleR = wheelAngleD / 180.0 * M_PI;//convert to radians
    // find the centre of the wheel position
    btTransform wheelTransform;
    wheelTransform.setRotation(btQuaternion(btVector3(0.0f, 0.0f, 1.0f), angleR));
    btVector3 wheelPos =
            btVector3(cos(angleR) * robSettings->radius, sin(angleR) * robSettings->radius, robSettings->wheelRadius) *
            SCALE;
    wheelTransform.setOrigin(wheelPos);
    //calculate moments of inertia of wheel
    btVector3 wheelInertia;
    wheelShape->calculateLocalInertia(robSettings->wheelMass, wheelInertia);
    // cosntruct rigid body
    //we need to multiply by body transform to construct the objects in the right places
    btDefaultMotionState *motionState = new btDefaultMotionState(hullTransform * wheelTransform);
    btRigidBody::btRigidBodyConstructionInfo wheelInfo(robSettings->wheelMass, motionState, wheelShape, wheelInertia);
    btRigidBody *wheel = new btRigidBody(wheelInfo);
    wheel->setUserIndex(bodyType::WHEEL);
    wheels[wheelLabel] = wheel;
    //construct joint/motor
    btVector3 heightOffset = btVector3(0, 0, -(robSettings->totalHeight+robSettings->bottomPlateHeight) * 0.5) *
                             SCALE;
    btHingeConstraint *constraint = new btHingeConstraint(*body, *wheel, wheelPos + heightOffset,
                                                          btVector3(0.0, 0.0, 0),
                                                          btVector3(wheelPos.x(), wheelPos.y(), 0),
                                                          btVector3(1.0, 0.0, 0.0));
    constraint->enableAngularMotor(true, 0, 10000000);
    constraint->setDbgDrawSize(1.5);
    // set friction to be different in each direction of the wheel axis x is perpendicular, y is tangent. Z friction is lateral (only really relevant for ball/ball and robot/robot collisions)
    wheel->setAnisotropicFriction(btVector3(0.15, 0.85, 0.0), btCollisionObject::CF_ANISOTROPIC_FRICTION);
    wheelMotor[wheelLabel] = constraint;
    //add everything to the world
    dynamicsWorld->addConstraint(constraint, true);
    dynamicsWorld->addRigidBody(wheel,COL_ROBOT,COL_EVERYTHING);

}
void SimBot::wheelControl(btScalar wheel0, btScalar wheel1, btScalar wheel2, btScalar wheel3) {
    wheelMotor[0]->setMotorTargetVelocity(wheel0);
    wheelMotor[1]->setMotorTargetVelocity(wheel1);
    wheelMotor[2]->setMotorTargetVelocity(wheel2);
    wheelMotor[3]->setMotorTargetVelocity(wheel3); //TODO: make 4 a constant called NWHEEL or so
}
void SimBot::localControl(btScalar velTangent, btScalar velNormal, btScalar velAngle) {
    btScalar degToRad = M_PI / 180.0f;
    btScalar angles[4] = {degToRad * robSettings->wheelAngle0,
                          degToRad * robSettings->wheelAngle1,
                          degToRad * robSettings->wheelAngle2,
                          degToRad * robSettings->wheelAngle3};
    for (int i = 0; i < 4; ++i) {
        btScalar wheelVel = 1.0 / robSettings->wheelRadius *
                            ((robSettings->radius * velAngle) - (velTangent * sin(angles[i])) +
                             (velNormal * cos(angles[i])));
        wheelMotor[i]->setMotorTargetVelocity(wheelVel);
    }
}
SimBot::SimBot(unsigned int id, std::unique_ptr<btDiscreteDynamicsWorld>& world, const std::unique_ptr<RobotSettings> &settings,
               const std::unique_ptr<WorldSettings>& worldSettings) : SimBot(id, world, settings, worldSettings,
                                                                      btVector3(0, 0, 0), 0.0) {
}
SimBot::~SimBot() {
    for (int i = shapes.size() - 1; i >= 0; --i) {
        delete shapes[i];
    }
    for (int j = 0; j < 4; ++j) {
        dynamicsWorld->removeRigidBody(wheels[j]);
        dynamicsWorld->removeConstraint(wheelMotor[j]);
        delete wheels[j];
        delete wheelMotor[j];
    }
    dynamicsWorld->removeConstraint(dribblerMotor);
    dynamicsWorld->removeRigidBody(dribbler);
    delete dribblerMotor;
    delete dribbler;
    dynamicsWorld->removeRigidBody(body);
    delete body;
    delete motionState;


}
unsigned int SimBot::getId() {
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
                //globalControlAngle(g.velx(),g.vely(),g.angle());
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
            std::cerr << "You shouldn't be seeing this!" << std::endl;
            break; //TODO: fix this error on startup
        }
    }
    //Kicker
    //TODO: listen to commands properly
    if (ball && canKickBall(ball)){
        std::cout<<"KICKING BALL"<<std::endl;
        btVector3 force=(ball->position()-body->getCenterOfMassPosition()).normalized()*1000;
        ball->kick(force);
    }
}
void SimBot::deactivate() {
    if (isActive) {
        wheelControl(0, 0, 0, 0);
        isActive = false;
    }
}
bool SimBot::canKickBall(SimBall *ball) {
    return (body->getCenterOfMassPosition() - ball->position()).norm() < 0.3 *SCALE;//TODO: fix actual kicker area
}

SSL_DetectionRobot SimBot::asDetection() const {
    SSL_DetectionRobot robot;
    robot.set_x(position().x()/SCALE*1000.0);//TODO: fix mm to m conversion. Do we only get up to mm precision (as integer) or is it double?
    robot.set_y(position().y()/SCALE*1000.0);
    robot.set_orientation(orientation());
    robot.set_height(0.144); //TODO: compute from robot values.
    robot.set_pixel_x(20.0);
    robot.set_pixel_y(24.0);
    robot.set_robot_id(id);
    return robot;
}