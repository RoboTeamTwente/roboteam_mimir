//
// Created by rolf on 19-09-19.
//

// The following is a great manual giving lots of tips hints and explanations on the library
// http://www.cs.kent.edu/~ruttan/GameEngines/lectures/Bullet_User_Manual
// this resource also has a lot of information:
// https://docs.google.com/document/d/10sXEhzFRSnvFcl3XxNGhnD4N2SedqwdAvK3dsihxVUA/edit#heading=h.2ye70wns7io3
// Also the cached wiki is useful, see:
// TODO: add link

#include "SimWorld.h"
#include "SimField.h"
#include "SimBall.h"
#include "../config/WorldSettings.h"
#include "../config/RobotSettings.h"
#include "CollisionShared.h"
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
    collisionDispatcher->setNearCallback(customNearCallback); //We use a custom callback so we can change some contact constraints (see CollisionShared)
    //btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    overlappingPairCache = std::make_unique<btDbvtBroadphase>();
    //the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    solver = std::make_unique<btSequentialImpulseConstraintSolver>();
    // the world in which all simulation happens
    dynamicsWorld = std::make_shared<btDiscreteDynamicsWorld>(collisionDispatcher.get(), overlappingPairCache.get(), solver.get(), collisionConfig.get());
    // make sure bullet calls our motor commands when relevant every physics tick
    dynamicsWorld->setInternalTickCallback(BulletTickCallback,this,true);
    resetWorld();
}
SimWorld::~SimWorld() {
    //delete bullet related objects in reverse order of creation!
}
btDiscreteDynamicsWorld* SimWorld::getWorld() {
    return dynamicsWorld.get(); // Raw as we don't want to share ownership of the world with e.g. visual interfaces
}
void SimWorld::stepSimulation() {
    dynamicsWorld->stepSimulation(1/200.0, 10, 1/200.0);
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
    dynamicsWorld->clearForces();//according to wiki
    //TODO: options for local, global velocity and angular control mode.
    //TODO: add delay options
    time += dt;
    for (const auto& command: blueCommands) {
        for (auto &robot : blueBots) {
            if (robot->getId()==command.id()){
                robot->receiveCommand(command,time);
            }
        }
    }
    for (const auto& command: yellowCommands) {
        for (auto &robot : yellowBots) {
            if (robot->getId()==command.id()){
                robot->receiveCommand(command,time);
            }
        }
    }
    blueCommands.clear();
    yellowCommands.clear();
    for (auto& bot : blueBots) {
//        bot->update(ball.get(),time);
        bot->globalControl(0.03,0.0,0.0); //TODO; remove after testing
    }
    for (auto& bot : yellowBots){
        bot->update(ball.get(),time);
    }
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
    // check if center of ball visible from camPoint (raycast) by intersections with robots
    // compute area in pixels as a function from distance to Camera. TODO: figure out if area computation takes into account the skew/distortion or if it's literally the raw pixel area
    //for later; add motionState interpolation

    std::vector<SSL_DetectionFrame> frames;
    for (const auto& camera: cameras) {
        SSL_DetectionFrame frame;
        frame.clear_balls();
        frame.clear_robots_blue();
        frame.clear_robots_yellow();
        frame.set_camera_id(camera.getId());
        frame.set_t_capture(time);//TODO: fix motionstate interpolation?
        frames.push_back(frame);
    }

    //TODO: add noise
    for (const auto &blueBot: blueBots) {
        const btVector3 botPos=blueBot->position();
        for (int i = 0; i < cameras.size(); ++ i) {
            if (cameras[i].isVisible(botPos.x(),botPos.y())){
                SSL_DetectionRobot bot=blueBot->asDetection();
                frames[i].add_robots_blue()->CopyFrom(bot);
            }
        }
    }
    for (const auto &yellowBot: yellowBots) {
        const btVector3 botPos=yellowBot->position();
        for (int j = 0; j < cameras.size(); ++ j) {
            if (cameras[j].isVisible(botPos.x(),botPos.y())) {
                SSL_DetectionRobot bot = yellowBot->asDetection();
                frames[j].add_robots_yellow()->CopyFrom(bot);
            }
        }
    }

    const btVector3 &ballPos=ball->position();
    for (int k = 0; k <cameras.size(); ++ k) {
        if (cameras[k].isBallVisible(ballPos)){
            frames[k].mutable_balls()->Add()->CopyFrom(ball->asDetection());
        }
    }

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
    if (tickCount%sendGeometryTicks == 0) { //TODO: make 120 not hardcoded but through interface/settings
        if (packets.empty()) {
            SSL_WrapperPacket wrapper;
            packets.push_back(wrapper);
        }
        packets[0].mutable_geometry()->CopyFrom(getGeometryData());
    }
    tickCount ++;
    return packets;
}

//TODO: use move semantics
void SimWorld::addCommands(std::vector<mimir_robotcommand> commands, bool TeamIsYellow) {
    if (TeamIsYellow){
        yellowCommands.insert(yellowCommands.end(),commands.begin(),commands.end());
    }
    else{
        blueCommands.insert(blueCommands.begin(),commands.begin(),commands.end());
    }
}
void SimWorld::setRobotCount(unsigned int numRobots, bool isYellow) {
    unsigned int current= isYellow ? numYellowBots : numBlueBots;
    if (current!=numRobots){
        if (isYellow){
            numYellowBots = numRobots;
        }
        else{
            numBlueBots = numRobots;
        }
        resetRobots();
    }
}

void SimWorld::resetRobots() {
    blueBots.clear();
    yellowBots.clear();
    for (unsigned int i = 0; i < numBlueBots; ++i) {
        blueBots.push_back(
                std::make_unique<SimBot>(i,dynamicsWorld, blueSettings, worldSettings,
                                         btVector3(0.0,0.0, 0.0)*worldSettings->scale, 0.0));
    }
    for (unsigned int i = 0; i < numYellowBots; ++i) {
        yellowBots.push_back(
                std::make_unique<SimBot>(i,dynamicsWorld, yellowSettings, worldSettings,
                                         btVector3(0.0,0.4, 0.0)*worldSettings->scale, 0.0));
    }
}
void SimWorld::resetWorld() {
    const btScalar SCALE=worldSettings->scale;
    dynamicsWorld->setGravity(
            btVector3(SCALE*worldSettings->gravityX, SCALE*worldSettings->gravityY, SCALE*worldSettings->gravityZ));
    field = std::make_shared<SimField>(dynamicsWorld, worldSettings);
    ball = std::make_shared<SimBall>(dynamicsWorld, worldSettings,
                                     btVector3(0.3*SCALE, 0, worldSettings->ballRadius*SCALE), btVector3(-SCALE*0.5, 0, 0));
    cameras.clear();
    cameras.push_back(Camera(btVector3(0.0,0.0,5.0)*SCALE,0.0,0.0,SCALE*14.0,SCALE*11.0,dynamicsWorld.get()));
    resetRobots();
}
void SimWorld::updateWorldConfig(std::shared_ptr<WorldSettings> _worldSettings) {
    worldSettings = std::make_shared<WorldSettings>(*_worldSettings);//We deference to take ownership
    resetWorld();
}
void SimWorld::updateRobotConfig(std::shared_ptr<RobotSettings> _robotSettings, bool isYellow) {
    if (isYellow){
        yellowSettings=std::make_shared<RobotSettings>(*_robotSettings);
    }
    else{
        blueSettings=std::make_shared<RobotSettings>(*_robotSettings);
    }
    resetRobots();
}
void SimWorld::setSendGeometryTicks(unsigned int ticks) {
    sendGeometryTicks = ticks;
}