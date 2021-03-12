//
// Created by rolf on 16-01-21.
//

#include "VisualizedSimBot.h"
#include <VisualizerUtil.h>

VisualizedSimBot::VisualizedSimBot(unsigned int _id,
                                   std::shared_ptr<btMultiBodyDynamicsWorld> world,
                                   const RobotSettings &settings,
                                   const WorldSettings &worldSettings,
                                   const btVector3 &initialPos,
                                   btScalar dir) : SimBot(_id, world, settings, worldSettings, initialPos, dir) {

  visual_wheel = std::make_unique<VisualizerCylinder>(settings.wheelThickness,settings.wheelRadius,1,1,1);
  wheel_material = Material(QVector3D(0.25,0.25,0.25),32.0);

  kicker_material = Material(QVector3D(0.3,0.3,0.3),32.0);
  visual_kicker = std::make_unique<VisualizerBox>(settings.kickerThickness,settings.kickerWidth,settings.kickerHeight,1,1,1);

  hull = std::make_unique<VisualizedRobotHull>(settings);
}
void VisualizedSimBot::init(QOpenGLShaderProgram *shader) {
  visual_wheel->init(shader);
  hull->init(shader);
  visual_kicker->init(shader);
}
void VisualizedSimBot::draw(QOpenGLShaderProgram *shader, QOpenGLFunctions *gl) {
  wheel_material.bind(shader);
  for (auto & wheel : wheels) {
    btTransform wheelTransform = wheel->getWorldTransform();
    QMatrix4x4 mat = getGLTransform(wheelTransform,1.0/SCALE);
    visual_wheel->draw(shader,gl,mat);
  }
  QMatrix4x4 transform = getGLTransform(body->getWorldTransform(),1.0/SCALE);
  hull->draw(shader,gl,transform);
  kicker_material.bind(shader);
  QMatrix4x4 model = getGLTransform(kickerWorldTransform(),1.0/SCALE);
  visual_kicker->draw(shader,gl,model);
}
void VisualizedSimBot::update(QOpenGLShaderProgram *shader, QOpenGLFunctions *gl) {
  if(!initialized){
    init(shader);
    initialized = true;
  }
  draw(shader,gl);
}
