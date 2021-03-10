//
// Created by rolf on 16-01-21.
//

#include "VisualizedSimBot.h"
VisualizedSimBot::VisualizedSimBot(unsigned int _id,
                                   std::shared_ptr<btMultiBodyDynamicsWorld> world,
                                   const RobotSettings &settings,
                                   const WorldSettings &worldSettings,
                                   const btVector3 &initialPos,
                                   btScalar dir) : SimBot(_id, world, settings, worldSettings, initialPos, dir) {

}
