//
// Created by rolf on 19-09-19.
//

// The following is a great manual giving lots of tips hints and explanations on the library
// http://www.cs.kent.edu/~ruttan/GameEngines/lectures/Bullet_User_Manual
// this resource also has a lot of information:
// https://docs.google.com/document/d/10sXEhzFRSnvFcl3XxNGhnD4N2SedqwdAvK3dsihxVUA/edit#heading=h.2ye70wns7io3

#include "SimWorld.h"
#include "SimField.h"
#include "SimBall.h"
#include "../config/WorldSettings.h"
#include "../config/RobotSettings.h"

static void BulletTickCallback(btDynamicsWorld * world, btScalar dt){
    SimWorld* simWorld = (SimWorld *) (world->getWorldUserInfo());// This is how it's done in the example. Seems like black magic
    simWorld->doCommands(dt);
}

SimWorld::SimWorld(std::shared_ptr<WorldSettings> _worldSettings, std::shared_ptr<RobotSettings> _blueSettings,
        std::shared_ptr<RobotSettings> _yellowSettings) {
    //We create local copies of the settings to ensure we are always sending the data back as the simulator sees it
    worldSettings = std::make_shared<WorldSettings>(*_worldSettings);
    blueSettings = std::make_shared<RobotSettings>(*_blueSettings);
    yellowSettings = std::make_shared<RobotSettings>(*_yellowSettings);
    //Contains default setup for memory and how collisions between different types of objects are handled/calculated
    collisionConfig = std::make_unique<btDefaultCollisionConfiguration>();

    //uses the default dispatcher. We might want to use the parallel one down the road.
    collisionDispatcher = std::make_unique<btCollisionDispatcher>(collisionConfig.get());

    //btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    overlappingPairCache = std::make_unique<btDbvtBroadphase>();

    //the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    solver = std::make_unique<btSequentialImpulseConstraintSolver>();

    // the world in which all simulation happens
    dynamicsWorld = std::make_shared<btDiscreteDynamicsWorld>(collisionDispatcher.get(), overlappingPairCache.get(), solver.get(), collisionConfig.get());
    const float SCALE = worldSettings->scale;
    dynamicsWorld->setGravity(
            btVector3(SCALE*worldSettings->gravityX, SCALE*worldSettings->gravityY, SCALE*worldSettings->gravityZ));
    // make sure bullet calls our motor commands when relevant every physics tick
    dynamicsWorld->setInternalTickCallback(BulletTickCallback,this,true);
    //field creates and manages all of the geometry related (static) physics objects in the world
    field = std::make_shared<SimField>(dynamicsWorld, worldSettings);
    //create a ball
    ball = std::make_shared<SimBall>(dynamicsWorld, worldSettings,
            btVector3(- 4*SCALE, 0, worldSettings->ballRadius*SCALE), btVector3(SCALE*8, 0, 0));
    //creating a robot for testing purposes TODO remove
    test = std::make_shared<SimBot>(dynamicsWorld, blueSettings, worldSettings,
            btVector3(0.0,0.0, 0.0)*worldSettings->scale, 0.0);

}
SimWorld::~SimWorld() {
    //delete bullet related objects in reverse order of creation!
}
btDiscreteDynamicsWorld* SimWorld::getWorld() {
    return dynamicsWorld.get(); // In general we don't want to share ownership of the world with e.g. visual interfaces
}
void SimWorld::stepSimulation() {
    dynamicsWorld->stepSimulation(1/100.0, 10, 1/600.0);
}
//helper functions for creating geometry
inline int scale(const float &meas) {
    return (int) (1000*meas);
}
void addLine(const std::string &name, float p1x, float p1y, float p2x, float p2y, SSL_GeometryFieldSize* field,
        float lineThickness) {
    SSL_FieldLineSegment segment;
    segment.set_name(name);
    segment.mutable_p1()->set_x(scale(p1x));
    segment.mutable_p1()->set_y(scale(p1y));
    segment.mutable_p2()->set_x(scale(p2x));
    segment.mutable_p2()->set_y(scale(p2y));
    segment.set_thickness(scale(lineThickness));
    field->add_field_lines()->CopyFrom(segment);
}
void addArc(const std::string &name, float centerx, float centery, float radius, float angle1, float angle2,
        SSL_GeometryFieldSize* field, float lineThickness) {
    SSL_FieldCicularArc arc;
    arc.set_name(name);
    arc.mutable_center()->set_x(scale(centerx));
    arc.mutable_center()->set_y(scale(centery));
    arc.set_radius(scale(radius));
    arc.set_a1(angle1); //TODO: check if arc angles from SSL-vision are in degrees or radians
    arc.set_a2(angle2);
    arc.set_thickness(scale(lineThickness));
    field->add_field_arcs()->CopyFrom(arc);
}
void SimWorld::doCommands(btScalar dt) {
    dynamicsWorld->clearForces();//according to wiki    //set robot motors
    //TODO: options for local, global velocity and angular control mode.
    test->localControl(1.0,0.0,0.0);
    dynamicsWorld->applyGravity();
}
SSL_GeometryData SimWorld::getGeometryData() {
    //TODO: add camera calibration info
    SSL_GeometryData data;
    SSL_GeometryFieldSize* geomField = data.mutable_field();
    //SSL geometry is sent in mm not in m
    //the names of the variables in the settings should correspond exactly with how the measurements from SSL-vision are done
    geomField->set_goal_depth(scale(worldSettings->goalDepth));
    geomField->set_field_length(scale(worldSettings->fieldLength));
    geomField->set_boundary_width(scale(worldSettings->boundaryWidth));
    geomField->set_field_width(scale(worldSettings->fieldWidth));
    geomField->set_goal_width(scale(worldSettings->goalWidth));

    const float hWidth = worldSettings->fieldWidth*0.5f;
    const float hLength = worldSettings->fieldLength*0.5f;
    const float defense = worldSettings->goalWidth;
    addLine("TopTouchLine", - hLength, hWidth, hLength, hWidth, geomField, worldSettings->lineWidth);
    addLine("BottomTouchLine", - hLength, - hWidth, hLength, - hWidth, geomField, worldSettings->lineWidth);
    addLine("LeftGoalLine", - hLength, - hWidth, - hLength, hWidth, geomField, worldSettings->lineWidth);
    addLine("RightGoalLine", hLength, - hWidth, hLength, hWidth, geomField, worldSettings->lineWidth);
    addLine("HalfwayLine", 0.0f, - hWidth, 0.0f, hWidth, geomField,
            worldSettings->lineWidth);//TODO: check if HalfwayLine and CenterLine are not accidentally swapped
    addLine("CenterLine", - hLength, 0.0f, hLength, 0.0f, geomField, worldSettings->lineWidth);

    addLine("LeftPenaltyStretch", - hLength + defense, - defense, - hLength + defense, defense, geomField,
            worldSettings->lineWidth);
    addLine("LeftFieldLeftPenaltyStretch", - hLength, defense, - hLength + defense, defense, geomField,
            worldSettings->lineWidth); //TODO: check what left/right mean in this context (w.r.t what view is this 'left'?)
    addLine("LeftFieldRightPenaltyStretch", - hLength, - defense, - hLength + defense, - defense, geomField,
            worldSettings->lineWidth);

    addLine("RightPenaltyStretch", hLength - defense, - defense, hLength - defense, defense, geomField,
            worldSettings->lineWidth);
    addLine("RightFieldLeftPenaltyStretch", hLength - defense, - defense, hLength, - defense, geomField,
            worldSettings->lineWidth);
    addLine("RightFieldRightPenaltyStretch", hLength - defense, defense, hLength, defense, geomField,
            worldSettings->lineWidth);

    addArc("CenterCircle", 0.0f, 0.0f, worldSettings->centerCircleRadius, 0.0f, 2*M_PI, geomField,
            worldSettings->lineWidth);
    return data;
}
std::vector<SSL_DetectionFrame> SimWorld::getDetectionFrames() {
    //pseudocode
    //make a detectionFrame for each camera
    // for each robot
    //find x,y+orientation
    //put info into relevant detectionFrames/camera's
    // for ball
    // find position
    // for each camera
    // check if center of ball visible from camPoint (raycast) by intersections with robots
    // compute area in pixels as a function from distance to Camera. TODO: figure out if area computation takes into account the skew/distortion or if it's literally the raw pixel area
    // if so add it to relevant detectionFrame
    //for later; add motionState interpolation

    std::vector<SSL_DetectionFrame> frames;
    SSL_DetectionFrame detFrame;
    SSL_DetectionBall detBall;
    btVector3 position = ball->position();
    detBall.set_x(scale(position.x())/worldSettings->scale);
    detBall.set_y(scale(position.y())/worldSettings->scale);
    detBall.set_area(0.0);//TODO: fix below 4 vars
    detBall.set_confidence(1.0);
    detBall.set_pixel_x(0);
    detBall.set_pixel_y(0);
    detBall.set_z(worldSettings->ballRadius);//TODO: figure out what's actually being sent by SSL-vision in this field
    detFrame.add_balls()->CopyFrom(detBall);

    //TODO: send robot from data
    SSL_DetectionRobot robot;
    robot.set_x(scale(test->position().x())/worldSettings->scale);
    robot.set_y(scale(test->position().y())/worldSettings->scale);
    robot.set_orientation(test->orientation());
    robot.set_height(0.144);
    robot.set_pixel_x(20.0);
    robot.set_pixel_y(24.0);
    robot.set_robot_id(1);
    detFrame.add_robots_blue()->CopyFrom(robot);
    robot.set_robot_id(2);
    detFrame.add_robots_yellow()->CopyFrom(robot);
    frames.push_back(detFrame);
    return frames;
}
std::vector<SSL_WrapperPacket> SimWorld::getPackets() {
    std::vector<SSL_WrapperPacket> packets;
    std::vector<SSL_DetectionFrame> frames = getDetectionFrames();
    for (const auto &frame : frames) {
        SSL_WrapperPacket wrapper;
        wrapper.mutable_detection()->CopyFrom(frame);
        packets.push_back(wrapper);
    }

    // we add the geometry every x frames
    if (tickCount%120 == 0) { //TODO: make 120 not hardcoded but through interface/settings
        if (packets.empty()) {
            SSL_WrapperPacket wrapper;
            packets.push_back(wrapper);
        }
        packets[0].mutable_geometry()->CopyFrom(getGeometryData());
    }
    tickCount ++;
    return packets;
}