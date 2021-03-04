//
// Created by rolf on 28-02-21.
//

#include "VisualizedSimField.h"

#include <utility>
#include <VisualizerUtil.h>
VisualizedSimField::VisualizedSimField(std::shared_ptr<btMultiBodyDynamicsWorld> world, const WorldSettings &cfg)
    : SimField(std::move(world), cfg) {
  scale = cfg.scale;
  btVector3 back_dimensions = goalBackDimensions(cfg)/scale;
  back_box =  std::make_unique<VisualizerBox>(back_dimensions.x(),back_dimensions.y(),back_dimensions.z(),0.5,0.5,0.5);
  back_transforms = goalBackTransforms(cfg);

  btVector3 side_dimensions = goalSideDimensions(cfg)/scale;
  side_box =  std::make_unique<VisualizerBox>(side_dimensions.x(),side_dimensions.y(),side_dimensions.z(),0.5,0.5,0.5);
  side_transforms = goalSideTransforms(cfg);

  ground_plane = std::make_unique<TexturedGroundPlane>(cfg.fieldLength,0.0,0.4,0.0);


  btScalar half_width = (cfg.fieldWidth*0.5+cfg.boundaryWidth);
  btScalar half_length = (cfg.fieldLength*0.5+cfg.boundaryWidth);
  btScalar wall_thickness = cfg.goalWallThickness*2.0;
  btScalar wall_height = cfg.goalHeight*2.0;
  btQuaternion i= btQuaternion::getIdentity();

  VisualizerBox length_wall(half_length,0.5f*wall_thickness,0.5f*wall_height,0.5,0.5,0.5);

  field_walls.emplace_back(std::make_unique<VisualizerBox>(half_length,0.5f*wall_thickness,0.5f*wall_height,0.5,0.5,0.5),
          btTransform (i,btVector3(0,half_width+0.5*wall_thickness,0.5*wall_height)));
  field_walls.emplace_back(std::make_unique<VisualizerBox>(half_length,0.5f*wall_thickness,0.5f*wall_height,0.5,0.5,0.5),
                           btTransform (i,btVector3(0,-(half_width+0.5*wall_thickness),0.5*wall_height)));

  field_walls.emplace_back(std::make_unique<VisualizerBox>(0.5f*wall_thickness,half_width+wall_thickness,0.5f*wall_height,0.5,0.5,0.5),
                           btTransform (i,btVector3(half_length+0.5*wall_thickness,0,0.5*wall_height)));

  field_walls.emplace_back(std::make_unique<VisualizerBox>(0.5f*wall_thickness,half_width+wall_thickness,0.5f*wall_height,0.5,0.5,0.5),
                           btTransform (i,btVector3(-(half_length+0.5*wall_thickness),0,0.5*wall_height)));

}
void VisualizedSimField::init(QOpenGLShaderProgram *shader) {
  back_box->init(shader);
  side_box->init(shader);
  ground_plane->init(shader);
  for(const auto& wall : field_walls){
    wall.first->init(shader);
  }

}
void VisualizedSimField::draw(QOpenGLShaderProgram *shader, QOpenGLFunctions *gl) {
  for(const auto& back_transform : back_transforms){
    QMatrix4x4 mat = getGLTransform(back_transform,1.0/scale);
    back_box->draw(shader,gl,mat);
  }

  for(const auto& transform : side_transforms){
    QMatrix4x4 mat = getGLTransform(transform,1.0/scale);
    side_box->draw(shader,gl,mat);
  }
  ground_plane->draw(shader,gl);

  for(const auto& wall : field_walls){
    QMatrix4x4 mat = getGLTransform(wall.second,1.0);
    wall.first->draw(shader,gl,mat);
  }
}