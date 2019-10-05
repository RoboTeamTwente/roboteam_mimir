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
    dynamicsWorld->setGravity(btVector3(worldSettings->gravityX,worldSettings->gravityY,worldSettings->gravityZ));

    //field creates and manages all of the geometry related (static) physics objects in the world
    field=new SimField(dynamicsWorld,worldSettings);
    //create a ball
    ball=new SimBall(dynamicsWorld,worldSettings);

}
SimWorld::~SimWorld() {
    //delete everything in reverse order of creation!
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
    dynamicsWorld->stepSimulation(1/60.0);
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
    arc.set_a1(angle1); //TODO: check if angles from SSL-vision are in degrees or radians
    arc.set_a2(angle2);
    arc.set_thickness(scale(lineThickness));
    field->add_field_arcs()->CopyFrom(arc);
}
SSL_GeometryData SimWorld::getGeometryData() {
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