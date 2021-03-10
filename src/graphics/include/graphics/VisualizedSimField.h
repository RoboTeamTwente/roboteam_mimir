//
// Created by rolf on 28-02-21.
//

#ifndef ROBOTEAM_MIMIR_SRC_GRAPHICS_INCLUDE_GRAPHICS_VISUALIZEDSIMFIELD_H_
#define ROBOTEAM_MIMIR_SRC_GRAPHICS_INCLUDE_GRAPHICS_VISUALIZEDSIMFIELD_H_

#include <simulator/SimField.h>
#include "VisualizerBox.h"
#include "TexturedGroundPlane.h"

class VisualizedSimField : public SimField{
 public:
  VisualizedSimField(std::shared_ptr<btMultiBodyDynamicsWorld> world, const WorldSettings& cfg);

  void init(QOpenGLShaderProgram * shader);
  void draw(QOpenGLShaderProgram * shader, QOpenGLFunctions * gl);
 protected:
  Material goal_material;

  Material wall_material;
  std::unique_ptr<TexturedGroundPlane> ground_plane;

  std::unique_ptr<VisualizerBox> back_box;
  std::vector<btTransform> back_transforms;

  std::unique_ptr<VisualizerBox> side_box;
  std::vector<btTransform> side_transforms;

  //visual only
  std::vector<std::pair<std::unique_ptr<VisualizerBox>,btTransform>> field_walls;

  //skybox
  float scale;
};

#endif //ROBOTEAM_MIMIR_SRC_GRAPHICS_INCLUDE_GRAPHICS_VISUALIZEDSIMFIELD_H_
