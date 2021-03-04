//
// Created by rolf on 19-09-19.
//

// The following is a great manual giving lots of tips hints and explanations on the library
// http://www.cs.kent.edu/~ruttan/GameEngines/lectures/Bullet_User_Manual
// this resource also has a lot of information:
// https://docs.google.com/document/d/10sXEhzFRSnvFcl3XxNGhnD4N2SedqwdAvK3dsihxVUA/edit#heading=h.2ye70wns7io3
// Also the cached wiki is useful, see:
// https://web.archive.org/web/20170706235814/http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Main_Page

#include "SimWorld.h"

#include "utilities/Random.h"
#include "CollisionShared.h"

void printVector(btVector3 vector) {
  std::cout << vector.x() << " " << vector.y() << " " << vector.z() << std::endl;
}
static void BulletPreTickCallback(btDynamicsWorld *world, btScalar dt) {
  SimWorld *simWorld =
      (SimWorld *) (world->getWorldUserInfo()); // black magic casting but this is what the library recommends.
  simWorld->doCommands(dt);
}

static void BulletPostTickCalback(btDynamicsWorld *world, btScalar dt) {
  SimWorld *simWorld = (SimWorld *) (world->getWorldUserInfo());
  simWorld->postProcess(dt);
}

SimWorld::SimWorld(const WorldSettings &_worldSettings,
                   const RobotSettings &_blueSettings,
                   const RobotSettings &_yellowSettings,
                   const SituationWorld &_situation) :
    blueSettings(_blueSettings),
    yellowSettings(_yellowSettings),
    worldSettings(_worldSettings),
    situation(_situation) {
  MaterialManager::initialize();
  setupMaterials();
  //We create local copies of all the inputs to ensure we are always sending the data back as the simulator sees it.
  //Contains default setup for memory and how collisions between different types of objects are handled/calculated
  collisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
  //uses the default dispatcher. We might want to use the parallel one down the road.
  collisionDispatcher = std::make_unique<btCollisionDispatcher>(collisionConfig.get());
  //We use a custom callback so we can change some contact constraints (see CollisionShared)
  //btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
  overlappingPairCache = std::make_unique<btDbvtBroadphase>();
  //the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)

  solver = std::make_unique<btMultiBodyConstraintSolver>();
  // the world in which all simulation happens
  dynamicsWorld = std::make_shared<btMultiBodyDynamicsWorld>(collisionDispatcher.get(), overlappingPairCache.get(),
                                                             solver.get(), collisionConfig.get());
  // make sure bullet calls our motor commands when relevant every physics tick
  dynamicsWorld->setInternalTickCallback(BulletPreTickCallback, this, true);
  dynamicsWorld->setInternalTickCallback(BulletPostTickCalback, this, false);

  delay = 0.0;
  robotVanishingProb = 0.0;
  ballVanishingProb = 0.0;
  random = std::make_unique<Random>(6.0, 6.0, 0.01, 6.0,
                                    6.0);//TODO: set these in interface : see http://www.cs.cmu.edu/~mmv/papers/03icra-jim.pdf for values
}
SimWorld::~SimWorld() {
  //delete bullet related objects in reverse order of creation!
  MaterialManager::deinitialize();
}
btDiscreteDynamicsWorld *SimWorld::getWorld() {
  return dynamicsWorld.get(); // Raw as we don't want to share ownership of the world with e.g. visual interfaces
}
std::vector<SSL_WrapperPacket> SimWorld::stepSimulation(const std::vector<mimir_robotcommand> &blue_commands,
                                                        const std::vector<mimir_robotcommand> &yellow_commands) {
  addCommands(blue_commands, false);
  addCommands(yellow_commands, true);
  dynamicsWorld->stepSimulation(timeStepSize, 1, timeStepSize);
  auto packets = getPackets();
  tickCount++;
  return packets;
}
//helper functions for creating geometry
inline int scale(const float &meas) {
  return (int) (1000 * meas);
}
void addLine(const std::string &name, float p1x, float p1y, float p2x, float p2y, SSL_GeometryFieldSize *field,
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
            SSL_GeometryFieldSize *field, float lineThickness) {
  SSL_FieldCircularArc arc;
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
    } else {
      command++;
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
    } else {
      command++;
    }
  }

  for (auto &bot : blueBots) {
    bot->update(ball.get(), time);
  }
  for (auto &bot : yellowBots) {
    bot->update(ball.get(), time);
  }
  ball->processKicks(dt); //Need dt becasue we use delta-v formulation

  std::cout << ball->velocity().length() / worldSettings.scale << std::endl;
  dynamicsWorld->applyGravity();// has to be done after everything else according to bullet wiki.
}
SSL_GeometryData SimWorld::getGeometryData() {

  SSL_GeometryData data;

  for (auto &camera : cameras) {
    data.add_calib()->CopyFrom(camera.asMessage());
  }

  SSL_GeometryFieldSize *geomField = data.mutable_field();
  //SSL geometry is sent in mm not in m
  //the names of the variables in the settings should correspond exactly with how the measurements from SSL-vision are done
  geomField->set_goal_depth(scale(worldSettings.goalDepth));
  geomField->set_field_length(scale(worldSettings.fieldLength));
  geomField->set_boundary_width(scale(worldSettings.boundaryWidth));
  geomField->set_field_width(scale(worldSettings.fieldWidth));
  geomField->set_goal_width(scale(worldSettings.goalWidth));

  const float hWidth = worldSettings.fieldWidth * 0.5f;
  const float hLength = worldSettings.fieldLength * 0.5f;
  const float defense = worldSettings.goalWidth;
  addLine("TopTouchLine", -hLength, hWidth, hLength, hWidth, geomField, worldSettings.lineWidth);
  addLine("BottomTouchLine", -hLength, -hWidth, hLength, -hWidth, geomField, worldSettings.lineWidth);
  addLine("LeftGoalLine", -hLength, -hWidth, -hLength, hWidth, geomField, worldSettings.lineWidth);
  addLine("RightGoalLine", hLength, -hWidth, hLength, hWidth, geomField, worldSettings.lineWidth);
  addLine("HalfwayLine", 0.0f, -hWidth, 0.0f, hWidth, geomField,
          worldSettings.lineWidth);//TODO: check if HalfwayLine and CenterLine are not accidentally swapped
  addLine("CenterLine", -hLength, 0.0f, hLength, 0.0f, geomField, worldSettings.lineWidth);

  addLine("LeftPenaltyStretch", -hLength + defense, -defense, -hLength + defense, defense, geomField,
          worldSettings.lineWidth);
  addLine("LeftFieldLeftPenaltyStretch", -hLength, defense, -hLength + defense, defense, geomField,
          worldSettings.lineWidth); //TODO: check what left/right mean in this context (w.r.t what view is this 'left'?)
  addLine("LeftFieldRightPenaltyStretch", -hLength, -defense, -hLength + defense, -defense, geomField,
          worldSettings.lineWidth);

  addLine("RightPenaltyStretch", hLength - defense, -defense, hLength - defense, defense, geomField,
          worldSettings.lineWidth);
  addLine("RightFieldLeftPenaltyStretch", hLength - defense, -defense, hLength, -defense, geomField,
          worldSettings.lineWidth);
  addLine("RightFieldRightPenaltyStretch", hLength - defense, defense, hLength, defense, geomField,
          worldSettings.lineWidth);

  addArc("CenterCircle", 0.0f, 0.0f, worldSettings.centerCircleRadius, 0.0f, 2 * M_PI, geomField,
         worldSettings.lineWidth);
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
    frame.set_t_sent(time);
    frame.set_frame_number(1);
    frames.push_back(frame);
  }

  for (const auto &blueBot: blueBots) {
    const btVector3 botPos = blueBot->position();
    addRobotToFrames(frames, blueBot, botPos, false);
  }
  for (const auto &yellowBot: yellowBots) {
    const btVector3 botPos = yellowBot->position();
    addRobotToFrames(frames, yellowBot, botPos, true);
  }
  addBallToFrames(frames);

  return frames;
}
//TODO: move parts to Robot class class
void SimWorld::addRobotToFrames(std::vector<SSL_DetectionFrame> &frames, const std::unique_ptr<SimBot> &bot,
                                const btVector3 &botPos, bool isYellow) {
  for (int i = 0; i < cameras.size(); ++i) {
    btVector3 imagePos = cameras[i].fieldToImage(botPos);
    if (random->getVanishing() > robotVanishingProb && cameras[i].isInImage(imagePos.x(), imagePos.y())) {
      SSL_DetectionRobot detection = bot->asDetection();
      detection.set_pixel_x(imagePos.x());
      detection.set_pixel_y(imagePos.y());
      btVector3 computedPos = cameras[i].imageToField(imagePos, bot->height()) / worldSettings.scale *
          1000;//in SSL-vision the robots are assumed to be on the ground
      detection.set_x(computedPos.x() + random->getX());
      detection.set_y(computedPos.y() + random->getY());
      detection.set_orientation(bot->orientation() + random->getOrientation());
      if (isYellow) {
        frames[i].add_robots_yellow()->CopyFrom(detection);
      } else {
        frames[i].add_robots_blue()->CopyFrom(detection);
      }
    }
  }
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
  if (tickCount % sendGeometryTicks == 0) {
    if (packets.empty()) {
      SSL_WrapperPacket wrapper;
      packets.push_back(wrapper);
    }
    packets[0].mutable_geometry()->CopyFrom(getGeometryData());
  }
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
    } else {
      numBlueBots = numRobots;
    }
    resetRobots();
  }
}

void SimWorld::resetWorld() {
  const btScalar SCALE = worldSettings.scale;
  dynamicsWorld->setGravity(
      btVector3(SCALE * worldSettings.gravityX, SCALE * worldSettings.gravityY,
                SCALE * worldSettings.gravityZ));
  resetField();
  resetBall();
  cameras.clear();//TODO: fix multiple camera's
  for (auto camerasettings : worldSettings.cameras) {
    cameras.emplace_back(camerasettings, dynamicsWorld.get(), SCALE);
  }
  resetRobots();

}
void SimWorld::reloadSituation() {
  const btScalar SCALE = worldSettings.scale;
  dynamicsWorld->setGravity(
      btVector3(SCALE * worldSettings.gravityX, SCALE * worldSettings.gravityY,
                SCALE * worldSettings.gravityZ));
  resetField();
  if (situation.ball) {
    resetBall(situation.ball->position * worldSettings.scale,
                                     situation.ball->velocity * worldSettings.scale);
    //TODO: make options for angular velocity e.g. rolling and such for ball construction
  }
  cameras.clear();//TODO: fix multiple camera's
  for (auto camerasettings : worldSettings.cameras) {
    cameras.emplace_back(camerasettings, dynamicsWorld.get(), SCALE);
  }
  numBlueBots = situation.blueBots.size(); //TODO: fix communication with interface and interactions between Simulator.
  numYellowBots = situation.yellowBots.size();
  resetRobots();

}
void SimWorld::resetRobots() {
  blueBots.clear();
  yellowBots.clear();

  for (int i = 0; i < numBlueBots; ++i) {
    if(i < situation.blueBots.size()){
      const auto& bot = situation.blueBots[i];
      blueBots.push_back(std::move(
          std::make_unique<SimBot>(bot.id, dynamicsWorld, blueSettings, worldSettings,
                                   bot.position * worldSettings.scale, bot.position.z())
      ));
    }else {
      int first_free_id = -1;
      for (int id= 0; id < numBlueBots+1; ++id) {
        bool id_used = false;
        for (const auto& bot : blueBots) {
          if(bot->getId() == id){
            id_used = true;
            break;
          }
        }
        if(!id_used){
          first_free_id = id;
          break;
        }
      }
      assert(first_free_id!=-1);
      btVector3 position = btVector3(0.5+first_free_id*0.5,0.5*worldSettings.fieldWidth,0.0) * worldSettings.scale;
      blueBots.push_back(std::move(
          std::make_unique<SimBot>(first_free_id,dynamicsWorld,blueSettings,worldSettings,position,0.0)));
    }
  }
  for (int i = 0; i < numYellowBots; ++i) {
    if(i < situation.yellowBots.size()){
      const auto& bot = situation.yellowBots[i];
      yellowBots.push_back(std::move(
          std::make_unique<SimBot>(bot.id, dynamicsWorld, yellowSettings, worldSettings,
                                   bot.position * worldSettings.scale, bot.position.z())
      ));
    }else{
      int first_free_id = -1;
      for (int id= 0; id < numYellowBots+1; ++id) {
        bool id_used = false;
        for (const auto& bot : yellowBots) {
          if(bot->getId() == id){
            id_used = true;
            break;
          }
        }
        if(!id_used){
          first_free_id = id;
          break;
        }
      }
      assert(first_free_id!=-1);
      btVector3 position = -btVector3(0.5+first_free_id*0.5,0.5*worldSettings.fieldWidth,0.0) * worldSettings.scale;
      yellowBots.push_back(std::move(
          std::make_unique<SimBot>(first_free_id,dynamicsWorld,yellowSettings,worldSettings,position,0.0)));
    }
  }
}
void SimWorld::setWorldSettings(const WorldSettings &_worldSettings) {
  worldSettings = _worldSettings;
  resetWorld();
}
void SimWorld::setRobotSettings(const RobotSettings &_robotSettings, bool isYellow) {
  if (isYellow) {
    yellowSettings = _robotSettings;
  } else {
    blueSettings = _robotSettings;
  }
  resetRobots();
}
void SimWorld::setSendGeometryTicks(unsigned int ticks) {
  sendGeometryTicks = ticks;
}

WorldSettings SimWorld::getWorldSettings() {
  return worldSettings;
}
RobotSettings SimWorld::getRobotSettings(bool isYellow) {
  return isYellow ? yellowSettings : blueSettings;
}
void SimWorld::setDelay(double _delay) {
  delay = _delay / 1000.0;// delay comes in in ms
}
void SimWorld::setRobotXNoise(double noise) {
  random->setXRange(noise * 1000); //TODO: fix scaling
}
void SimWorld::setRobotYNoise(double noise) {
  random->setYRange(noise * 1000);
}
void SimWorld::setRobotOrNoise(double noise) {
  random->setOrientationRange(noise);
}
void SimWorld::setRobotVanishing(double prob) {
  robotVanishingProb = prob;
}
void SimWorld::setBallXNoise(double noise) {
  random->setBallXRange(noise * 1000);
}
void SimWorld::setBallYNoise(double noise) {
  random->setBallYRange(noise * 1000);
}
void SimWorld::setBallVanishing(double prob) {
  ballVanishingProb = prob;
}

//TODO: move parts to ball class
void SimWorld::addBallToFrames(std::vector<SSL_DetectionFrame> &frames) {
  if (!ball) {
    return;
  }
  const btVector3 &ballPos = ball->position();
  for (int i = 0; i < cameras.size(); ++i) {
    btVector3 imagePos = cameras[i].fieldToImage(ballPos);
    if (random->getVanishing() > ballVanishingProb &&
        cameras[i].isInImage(imagePos.x(), imagePos.y()) &&
        cameras[i].isBallVisible(ballPos)) {
      SSL_DetectionBall detBall = ball->asDetection();
      btVector3 computedPos = cameras[i].imageToField(imagePos, ball->radius()) / worldSettings.scale * 1000;
      detBall.set_x(computedPos.x() + random->getBallX());
      detBall.set_y(computedPos.y() + random->getBallY());
      detBall.set_pixel_x(imagePos.x());
      detBall.set_pixel_y(imagePos.y());
      //Alternative:
//            btVector3 pos=cameras[i].extrapolation(ballPos,ball->radius()) / worldSettings.scale * 1000;;
//            detBall.set_x(pos.x());
//            detBall.set_y(pos.y());

      frames[i].mutable_balls()->Add()->CopyFrom(detBall);
    }
  }
}
void SimWorld::setupMaterials() {
  Material ball_ground_contact;
  ball_ground_contact.friction = 0.35;
  ball_ground_contact.rollingFriction =
      0.106607348; // TODO: fix scaling issues? (e.g. doesn't scale well with ball mass/world size right now)
  //ball_ground_contact.rollingFriction = 0.5;
  ball_ground_contact.spinningFriction = 0.03; //TODO: measure, this is just a random estimate
  ball_ground_contact.restitution = 0.55; //TODO: use measurements (guestimate now)
  MaterialManager::setMaterial(COL_BALL, COL_GROUND, ball_ground_contact);

  Material robot_hull_ball_contact;
  robot_hull_ball_contact.friction = 0.22;
  robot_hull_ball_contact.restitution = 0.6;
  MaterialManager::setMaterial(COL_BALL, COL_ROBOT_HULL, robot_hull_ball_contact);

  Material robot_dribbler_ball_contact;
  robot_dribbler_ball_contact.friction = 0.0;
  robot_dribbler_ball_contact.restitution = 1.0;
  MaterialManager::setMaterial(COL_BALL, COL_ROBOT_DRIBBLER, robot_dribbler_ball_contact);

  Material ball_wall_contact;
  ball_wall_contact.rollingFriction = 3 * ball_ground_contact.rollingFriction.value();
  ball_wall_contact.friction = 0.5;
  ball_wall_contact.restitution = 0.25;
  MaterialManager::setMaterial(COL_BALL, COL_WALL, robot_hull_ball_contact);

  Material wheel_ground_contact;
  wheel_ground_contact.frictionCFM = 0.002;
  wheel_ground_contact.contactCFM = 0.002;
  MaterialManager::setMaterial(COL_ROBOT_WHEEL, COL_GROUND, wheel_ground_contact);

  Material hull_ground_contact;
  hull_ground_contact.friction = 1.0;
  hull_ground_contact.restitution = 0.0;
  MaterialManager::setMaterial(COL_ROBOT_HULL, COL_GROUND, hull_ground_contact);

  //MaterialManager::setMaterial();

  //robot wall
  //robot robot

}
void SimWorld::postProcess(btScalar dt) {
//  bool isCollision = false;
//  int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
//  btPersistentManifold * manifold;
//  for (int i = 0; i < numManifolds; i++)
//  {
//    btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
//    const btCollisionObject* obA = contactManifold->getBody0();
//    const btCollisionObject* obB = contactManifold->getBody1();
//
//    if (!obA->getCollisionShape()->isNonMoving() && !obB->getCollisionShape()->isNonMoving()) {
//      isCollision = true;
//      manifold = contactManifold;
//    }
//  }
//  if(isCollision){
//    std::cout<<"COLLISION"<<std::endl;
//  }
//  printVector(ball->velocity()/worldSettings.scale);

  //
}
void SimWorld::resetBall() {
  ball = std::make_unique<SimBall>(dynamicsWorld, worldSettings);
}
void SimWorld::resetBall(btVector3 initialPos, btVector3 initialVel) {
  ball = std::make_unique<SimBall>(dynamicsWorld, worldSettings, situation.ball->position * worldSettings.scale,
                                   situation.ball->velocity * worldSettings.scale);

}
void SimWorld::resetField() {
  field = std::make_unique<SimField>(dynamicsWorld, worldSettings);
}
