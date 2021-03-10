//
// Created by rolf on 16-01-21.
//

#ifndef ROBOTEAM_MIMIR_VISUALIZEDBALL_H
#define ROBOTEAM_MIMIR_VISUALIZEDBALL_H

#include <simulator/SimBall.h>
#include "VisualizerBall.h"
#include "Material.h"

class VisualizedSimBall : public SimBall {
 public:
  VisualizedSimBall(std::shared_ptr<btMultiBodyDynamicsWorld> world, const WorldSettings & settings);
  VisualizedSimBall(std::shared_ptr<btMultiBodyDynamicsWorld> world, const WorldSettings & settings, btVector3 initial_pos,
                    btVector3 initial_vel);
  ~VisualizedSimBall() override;

  void init(QOpenGLShaderProgram * shader);
  void draw(QOpenGLShaderProgram * shader, QOpenGLFunctions * gl);
 protected:
  std::unique_ptr<VisualizerBall> visual;
  Material material;
};


#endif //ROBOTEAM_MIMIR_VISUALIZEDBALL_H
