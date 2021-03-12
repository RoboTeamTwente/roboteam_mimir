//
// Created by rolf on 16-01-21.
//

#ifndef ROBOTEAM_MIMIR_VISUALIZEDSIMBOT_H
#define ROBOTEAM_MIMIR_VISUALIZEDSIMBOT_H

#include <simulator/SimBot.h>
#include "VisualizerCylinder.h"
#include "VisualizerBox.h"
#include "Material.h"
#include "VisualizedRobotHull.h"

 class VisualizedSimBot : public SimBot {
 public:
  VisualizedSimBot(unsigned int _id,  std::shared_ptr<btMultiBodyDynamicsWorld> world,
                                  const RobotSettings &settings,
                                  const WorldSettings &worldSettings,
                                  const btVector3 &initialPos,
                                  btScalar dir);


   void update(QOpenGLShaderProgram * shader,QOpenGLFunctions * gl);
 private:
   void init(QOpenGLShaderProgram * shader);
   void draw(QOpenGLShaderProgram * shader, QOpenGLFunctions * gl);

  bool initialized = false;
  Material wheel_material;
  std::unique_ptr<VisualizerCylinder> visual_wheel;

  Material kicker_material;
  std::unique_ptr<VisualizerBox> visual_kicker;

  std::unique_ptr<VisualizedRobotHull> hull;
};


#endif //ROBOTEAM_MIMIR_VISUALIZEDSIMBOT_H
