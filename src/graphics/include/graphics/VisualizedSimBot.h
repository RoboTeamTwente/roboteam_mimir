//
// Created by rolf on 16-01-21.
//

#ifndef ROBOTEAM_MIMIR_VISUALIZEDSIMBOT_H
#define ROBOTEAM_MIMIR_VISUALIZEDSIMBOT_H

#include <simulator/SimBot.h>
#include "VisualizerCylinder.h"
#include "VisualizerBox.h"
 class VisualizedSimBot : public SimBot {
 public:
  VisualizedSimBot(unsigned int _id,  std::shared_ptr<btMultiBodyDynamicsWorld> world,
                                  const RobotSettings &settings,
                                  const WorldSettings &worldSettings,
                                  const btVector3 &initialPos,
                                  btScalar dir);
 private:
  std::unique_ptr<VisualizerCylinder> visual_wheel;

  std::unique_ptr<VisualizerBox> visual_kicker;

  //std::unique_ptr<> robot_mesh;
};


#endif //ROBOTEAM_MIMIR_VISUALIZEDSIMBOT_H
