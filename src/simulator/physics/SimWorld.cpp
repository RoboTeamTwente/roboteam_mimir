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
const float SCALE=200;
SimWorld::SimWorld(WorldSettings* _worldSettings,RobotSettings* _blueSettings,RobotSettings* _yellowSettings) {
    //We create local copies of the settings to ensure we are always sending the data back as the simulator sees it
    worldSettings = new WorldSettings(*_worldSettings);
    blueSettings = new RobotSettings(*_blueSettings);
    yellowSettings= new RobotSettings(*_yellowSettings);
    //Contains default setup for memory and how collisions between different types of objects are handled/calculated
    collisionConfig = new btDefaultCollisionConfiguration();

    //uses the default dispatcher. We might want to use the parallel one down the road.
    collisionDispatcher = new btCollisionDispatcher(collisionConfig);

    //btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    overlappingPairCache = new btDbvtBroadphase();

    //the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    solver = new btSequentialImpulseConstraintSolver();

    // the world in which all simulation happens
    dynamicsWorld= new btDiscreteDynamicsWorld(collisionDispatcher,overlappingPairCache,solver,collisionConfig);
    dynamicsWorld->setGravity(btVector3(SCALE*worldSettings->gravityX,SCALE*worldSettings->gravityY,SCALE*worldSettings->gravityZ));

    //field creates and manages all of the geometry related (static) physics objects in the world
    field=new SimField(dynamicsWorld,worldSettings);
    //create a ball
    ball=new SimBall(dynamicsWorld,worldSettings,btVector3(0,SCALE,0.7*SCALE),btVector3(0,-SCALE,0));
    //creating a robot for testing purposes TODO remove
    test=new SimBot(dynamicsWorld,blueSettings);

}
SimWorld::~SimWorld() {
    delete worldSettings;
    delete blueSettings;
    delete yellowSettings;
    //delete bullet related objects in reverse order of creation!
    delete field;
    delete dynamicsWorld;
    delete solver;
    delete overlappingPairCache;
    delete collisionDispatcher;
    delete collisionConfig;
}
btDiscreteDynamicsWorld* SimWorld::getWorld() {
    return dynamicsWorld;
}
void SimWorld::stepSimulation() {
    dynamicsWorld->stepSimulation(1/200.0,10,1/200.0);
}
//helper functions for creating geometry
inline int scale(const float &meas){
    return (int) (1000*meas);
}
void addLine(const std::string& name,float p1x,float p1y, float p2x,float p2y, SSL_GeometryFieldSize* field, float lineThickness){
    SSL_FieldLineSegment segment;
    segment.set_name(name);
    segment.mutable_p1()->set_x(scale(p1x));
    segment.mutable_p1()->set_y(scale(p1y));
    segment.mutable_p2()->set_x(scale(p2x));
    segment.mutable_p2()->set_y(scale(p2y));
    segment.set_thickness(scale(lineThickness));
    field->add_field_lines()->CopyFrom(segment);
}
void addArc(const std::string& name, float centerx, float centery, float radius,float angle1,float angle2,SSL_GeometryFieldSize* field,float lineThickness){
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
SSL_GeometryData SimWorld::getGeometryData() {
    //TODO: add camera calibration info
    SSL_GeometryData data;
    SSL_GeometryFieldSize* geomField=data.mutable_field();
    //SSL geometry is sent in mm not in m
    //the names of the variables in the settings should correspond exactly with how the measurements from SSL-vision are done
    geomField->set_goal_depth(scale(worldSettings->goalDepth));
    geomField->set_field_length(scale(worldSettings->fieldLength));
    geomField->set_boundary_width(scale(worldSettings->boundaryWidth));
    geomField->set_field_width(scale(worldSettings->fieldWidth));
    geomField->set_goal_width(scale(worldSettings->goalWidth));

    const float hWidth=worldSettings->fieldWidth*0.5f;
    const float hLength=worldSettings->fieldLength*0.5f;
    const float defense=worldSettings->goalWidth;
    addLine("TopTouchLine",-hLength,hWidth,hLength,hWidth,geomField,worldSettings->lineWidth);
    addLine("BottomTouchLine",-hLength,-hWidth,hLength,-hWidth,geomField,worldSettings->lineWidth);
    addLine("LeftGoalLine",-hLength,-hWidth,-hLength,hWidth,geomField,worldSettings->lineWidth);
    addLine("RightGoalLine",hLength,-hWidth,hLength,hWidth,geomField,worldSettings->lineWidth);
    addLine("HalfwayLine",0.0f,-hWidth,0.0f,hWidth,geomField,worldSettings->lineWidth);//TODO: check if HalfwayLine and CenterLine are not accidentally swapped
    addLine("CenterLine",-hLength,0.0f,hLength,0.0f,geomField,worldSettings->lineWidth);

    addLine("LeftPenaltyStretch",-hLength+defense,-defense,-hLength+defense,defense,geomField,worldSettings->lineWidth);
    addLine("LeftFieldLeftPenaltyStretch",-hLength,defense,-hLength+defense,defense,geomField,worldSettings->lineWidth);
    addLine("LeftFieldRightPenaltyStretch",-hLength,-defense,-hLength+defense,-defense,geomField,worldSettings->lineWidth);

    addLine("RightPenaltyStretch",hLength-defense,-defense,hLength-defense,defense,geomField,worldSettings->lineWidth);
    addLine("RightFieldLeftPenaltyStretch",hLength-defense,-defense,hLength,-defense,geomField,worldSettings->lineWidth);
    addLine("RightFieldRightPenaltyStretch",hLength-defense,defense,hLength,defense,geomField,worldSettings->lineWidth);

    addArc("CenterCircle",0.0f,0.0f,worldSettings->centerCircleRadius,0.0f,2*M_PI,geomField,worldSettings->lineWidth);
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
    btVector3 position=ball->position();
    detBall.set_x(position.x());
    detBall.set_y(position.y());
    detBall.set_area(0.0);//TODO: fix below 4 vars
    detBall.set_confidence(1.0);
    detBall.set_pixel_x(0);
    detBall.set_pixel_y(0);
    detBall.set_z(worldSettings->ballRadius);//TODO: figure out what's actually being sent by SSL-vision in this field
    detFrame.add_balls()->CopyFrom(detBall);
    frames.push_back(detFrame);
    //std::cout<<ball->position().x()/SCALE<<":" <<ball->position().y()/SCALE<<std::endl;
    std::cout<<sqrt(ball->velocity().x()*ball->velocity().x()+ball->velocity().y()*ball->velocity().y())/SCALE<<std::endl;
    return frames;
}
std::vector<SSL_WrapperPacket> SimWorld::getPackets() {
    std::vector<SSL_WrapperPacket> packets;
    std::vector<SSL_DetectionFrame> frames=getDetectionFrames();
    for (const auto& frame : frames) {
        SSL_WrapperPacket wrapper;
        wrapper.mutable_detection()->CopyFrom(frame);
        packets.push_back(wrapper);
    }
    if(tickCount%120==0){ //TODO: make 120 not hardcoded
        if(packets.empty()){
            SSL_WrapperPacket wrapper;
            packets.push_back(wrapper);
        }
        packets[0].mutable_geometry()->CopyFrom(getGeometryData());
    }
    tickCount++;
    return packets;
}