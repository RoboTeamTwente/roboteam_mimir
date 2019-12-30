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
#include "../utilities/Random.h"
#include "../config/WorldConfig.h" //TODO: fix these includes
#include "../config/RobotConfig.h"
#include "../config/Situation.h"
#include "CollisionShared.h"

static void BulletTickCallback(btDynamicsWorld* world, btScalar dt) {
    SimWorld* simWorld = (SimWorld*) (world->getWorldUserInfo());// This is how it's done in the example. Seems like black magic
    simWorld->doCommands(dt);
}

SimWorld::SimWorld(const std::unique_ptr<WorldConfig> &_worldSettings,
        const std::unique_ptr<RobotConfig> &_blueSettings,
        const std::unique_ptr<RobotConfig> &_yellowSettings,
        const std::unique_ptr<SituationWorld> &_situation) {
    //We create local copies of all the inputs to ensure we are always sending the data back as the simulator sees it.
    worldSettings = std::make_unique<WorldSettings>(*_worldSettings->settings);
    blueSettings = std::make_unique<RobotSettings>(*_blueSettings->settings);
    yellowSettings = std::make_unique<RobotSettings>(*_yellowSettings->settings);
    situation = std::make_unique<SituationWorld>(*_situation);
    //Contains default setup for memory and how collisions between different types of objects are handled/calculated
    collisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
    //uses the default dispatcher. We might want to use the parallel one down the road.
    collisionDispatcher = std::make_unique<btCollisionDispatcher>(collisionConfig.get());
    collisionDispatcher->setNearCallback(
            customNearCallback); //We use a custom callback so we can change some contact constraints (see CollisionShared)
    //btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    overlappingPairCache = std::make_unique<btDbvtBroadphase>();
    //the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    solver = std::make_unique<btMultiBodyConstraintSolver>();
    // the world in which all simulation happens
    dynamicsWorld = std::make_unique<btMultiBodyDynamicsWorld>(collisionDispatcher.get(), overlappingPairCache.get(),
            solver.get(), collisionConfig.get());
    // make sure bullet calls our motor commands when relevant every physics tick
    dynamicsWorld->setInternalTickCallback(BulletTickCallback, this, true);

    delay = 0.0;
    robotVanishingProb = 0.1;
    ballVanishingProb = 0.1;
    random = std::make_unique<Random>(6.0, 6.0, 0.01, 6.0,
            6.0);//TODO: set these in interface : see http://www.cs.cmu.edu/~mmv/papers/03icra-jim.pdf for values
    reloadSituation();
}
SimWorld::~SimWorld() {
    //delete bullet related objects in reverse order of creation!
}
btDiscreteDynamicsWorld* SimWorld::getWorld() {
    return dynamicsWorld.get(); // Raw as we don't want to share ownership of the world with e.g. visual interfaces
}
void SimWorld::stepSimulation(double dt) {
    dynamicsWorld->stepSimulation(dt, 1, dt);
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
    time += dt;
    for (auto command = blueCommands.begin(); command != blueCommands.end();) {
        if (time >= command->simulatorReceiveTime + delay) {
            for (auto &robot : blueBots) {
                if (robot->getId() == command->command.id()) {
                    robot->receiveCommand(command->command, time);
                }
            }
            command = blueCommands.erase(command);
        }
        else {
            command ++;
        }
    }
    for (auto command = yellowCommands.begin(); command != yellowCommands.end();) {
        if (time >= command->simulatorReceiveTime + delay) {
            for (auto &robot : yellowBots) {
                if (robot->getId() == command->command.id()) {
                    robot->receiveCommand(command->command, time);
                }
            }
            command = yellowCommands.erase(command);
        }
        else {
            command ++;
        }
    }

    for (auto &bot : blueBots) {
        bot->update(ball.get(), time);
    }
    for (auto &bot : yellowBots) {
        bot->update(ball.get(), time);
    }
    dynamicsWorld->applyGravity();// has to be done after everything else according to bullet wiki.
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
    // compute area in pixels as a function from distance to Camera. TODO: figure out if area computation takes into account the skew/distortion or if it's literally the raw pixel area
    //for later; add motionState interpolation

    std::vector<SSL_DetectionFrame> frames;
    for (const auto &camera: cameras) {
        SSL_DetectionFrame frame;
        frame.clear_balls();
        frame.clear_robots_blue();
        frame.clear_robots_yellow();
        frame.set_camera_id(camera.getId());
        frame.set_t_capture(time);//TODO: fix motionstate interpolation?
        frames.push_back(frame);
    }

    for (const auto &blueBot: blueBots) {
        const btVector3 botPos = blueBot->position();
        for (int i = 0; i < cameras.size(); ++ i) {
            if (random->getVanishing() > robotVanishingProb && cameras[i].isVisible(botPos.x(), botPos.y())) {
                SSL_DetectionRobot bot = blueBot->asDetection();
                bot.set_x(bot.x() + random->getX());
                bot.set_y(bot.y() + random->getY());
                bot.set_orientation(bot.orientation() + random->getOrientation());
                frames[i].add_robots_blue()->CopyFrom(bot);
            }
        }
    }
    for (const auto &yellowBot: yellowBots) {
        const btVector3 botPos = yellowBot->position();
        for (int j = 0; j < cameras.size(); ++ j) {
            if (random->getVanishing() > robotVanishingProb && cameras[j].isVisible(botPos.x(), botPos.y())) {
                SSL_DetectionRobot bot = yellowBot->asDetection();
                bot.set_x(bot.x() + random->getX());
                bot.set_y(bot.y() + random->getY());
                bot.set_orientation(bot.orientation() + random->getOrientation());
                frames[j].add_robots_yellow()->CopyFrom(bot);
            }
        }
    }
    if (ball) {
        const btVector3 &ballPos = ball->position();
        for (int k = 0; k < cameras.size(); ++ k) {
            if (random->getVanishing() > ballVanishingProb && cameras[k].isBallVisible(ballPos)) {
                SSL_DetectionBall detBall = ball->asDetection();
                detBall.set_x(detBall.x() + random->getBallX());
                detBall.set_y(detBall.y() + random->getBallY());
                frames[k].mutable_balls()->Add()->CopyFrom(detBall);
            }
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
    if (tickCount%sendGeometryTicks == 0) {
        if (packets.empty()) {
            SSL_WrapperPacket wrapper;
            packets.push_back(wrapper);
        }
        packets[0].mutable_geometry()->CopyFrom(getGeometryData());
    }
    tickCount ++;
    return packets;
}

//TODO: use move semantics?
void SimWorld::addCommands(const std::vector<mimir_robotcommand> &commands, bool TeamIsYellow) {
    std::vector<RobotCommand> &commandVector = TeamIsYellow ? yellowCommands : blueCommands;
    for (const auto &command : commands) {
        commandVector.emplace_back(RobotCommand(command, time));
    }
}
void SimWorld::setRobotCount(unsigned int numRobots, bool isYellow) {
    unsigned int current = isYellow ? numYellowBots : numBlueBots;
    if (current != numRobots) {
        if (isYellow) {
            numYellowBots = numRobots;
        }
        else {
            numBlueBots = numRobots;
        }
        resetRobots();
    }
}

void SimWorld::resetRobots() {
    blueBots.clear();
    yellowBots.clear();
    for (unsigned int i = 0; i < numBlueBots; ++ i) {
        blueBots.push_back(std::move(
                std::make_unique<SimBot>(i, dynamicsWorld, blueSettings, worldSettings,
                        btVector3(0.0, 0.0, 0.0)*worldSettings->scale, 0.0)));
    }
    for (unsigned int i = 0; i < numYellowBots; ++ i) {
        yellowBots.push_back(std::move(
                std::make_unique<SimBot>(i, dynamicsWorld, yellowSettings, worldSettings,
                        btVector3(0.0, 0.4, 0.0)*worldSettings->scale, 0.0)));
    }
}
void SimWorld::resetWorld() {
    const btScalar SCALE = worldSettings->scale;
    dynamicsWorld->setGravity(
            btVector3(SCALE*worldSettings->gravityX, SCALE*worldSettings->gravityY,
                    SCALE*worldSettings->gravityZ));
    field = std::make_unique<SimField>(dynamicsWorld, worldSettings);
    ball = std::make_unique<SimBall>(dynamicsWorld, worldSettings);
    cameras.clear();//TODO: fix multiple camera's
    cameras.push_back(
            Camera(btVector3(0.0, 0.0, 5.0)*SCALE, 0.0, 0.0, SCALE*14.0, SCALE*11.0, dynamicsWorld.get()));
    resetRobots();
}
void SimWorld::reloadSituation() {
    const btScalar SCALE = worldSettings->scale;
    dynamicsWorld->setGravity(
            btVector3(SCALE*worldSettings->gravityX, SCALE*worldSettings->gravityY,
                    SCALE*worldSettings->gravityZ));
    field = std::make_unique<SimField>(dynamicsWorld, worldSettings);
    if (situation->ball) {
        ball = std::make_unique<SimBall>(dynamicsWorld, worldSettings, situation->ball->position*worldSettings->scale,
                situation->ball->velocity*worldSettings->scale);
        //TODO: make options for angular velocity e.g. rolling and such for ball construction
    }
    cameras.clear();//TODO: fix multiple camera's


    cameras.push_back(
            Camera(btVector3(0.0, 0.0, 5.0)*SCALE, 0.0, 0.0, SCALE*14.0, SCALE*11.0, dynamicsWorld.get()));
    cameras[0].imageToField(btVector3(1000,1000,0),147);
    blueBots.clear();
    yellowBots.clear();
    for (const auto &bot : situation->blueBots) {
        blueBots.push_back(std::move(
                std::make_unique<SimBot>(bot.id, dynamicsWorld, blueSettings, worldSettings,
                        bot.position*worldSettings->scale, bot.position.z())
        ));
    }
    for (const auto &bot : situation->yellowBots) {
        yellowBots.push_back(std::move(
                std::make_unique<SimBot>(bot.id, dynamicsWorld, blueSettings, worldSettings,
                        bot.position*worldSettings->scale, bot.position.z())
        ));
    }
    numBlueBots = situation->blueBots.size(); //TODO: fix communication with interface and interactions between Simulator.
    numYellowBots = situation->yellowBots.size();
}
void SimWorld::updateWorldConfig(const std::unique_ptr<WorldConfig> &_worldSettings) {
    worldSettings = std::make_unique<WorldSettings>(*_worldSettings->settings);
    resetWorld();
}
void SimWorld::updateRobotConfig(const std::unique_ptr<RobotConfig> &_robotSettings, bool isYellow) {
    if (isYellow) {
        yellowSettings = std::make_unique<RobotSettings>(*_robotSettings->settings);
    }
    else {
        blueSettings = std::make_unique<RobotSettings>(*_robotSettings->settings);
    }
    resetRobots();
}
void SimWorld::setSendGeometryTicks(unsigned int ticks) {
    sendGeometryTicks = ticks;
}

WorldSettings* SimWorld::getWorldSettings() {
    return worldSettings.get();
}
RobotSettings* SimWorld::getRobotSettings(bool isYellow) {
    return isYellow ? yellowSettings.get() : blueSettings.get();
}
void SimWorld::setDelay(double _delay) {
    delay = _delay/1000.0;// delay comes in in ms
}
void SimWorld::setRobotXNoise(double noise) {
    random->setXRange(noise*1000); //TODO: fix scaling
}
void SimWorld::setRobotYNoise(double noise) {
    random->setYRange(noise*1000);
}
void SimWorld::setRobotOrNoise(double noise) {
    random->setOrientationRange(noise);
}
void SimWorld::setRobotVanishing(double prob) {
    robotVanishingProb = prob;
}
void SimWorld::setBallXNoise(double noise) {
    random->setBallXRange(noise*1000);
}
void SimWorld::setBallYNoise(double noise) {
    random->setBallYRange(noise*1000);
}
void SimWorld::setBallVanishing(double prob) {
    ballVanishingProb = prob;
}