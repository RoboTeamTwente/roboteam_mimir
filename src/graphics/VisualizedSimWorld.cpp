//
// Created by rolf on 23-11-20.
//

#include <VisualizedSimField.h>
#include "VisualizedSimWorld.h"
#include "VisualizedSimBall.h"
#include "VisualizedSimBot.h"

VisualizedSimWorld::VisualizedSimWorld(const WorldSettings &world_settings,
                                       const RobotSettings &blue,
                                       const RobotSettings &yellow,
                                       const SituationWorld &situation) : SimWorld(world_settings,
                                                                                   blue,
                                                                                   yellow,
                                                                                   situation) {

}
void VisualizedSimWorld::draw(QOpenGLShaderProgram *shader, QOpenGLFunctions *gl) {
  shader->setUniformValue("compute_lighting",true);

  getVisualBall()->draw(shader,gl);

  getVisualField()->draw(shader,gl);
}
void VisualizedSimWorld::init_gl(QOpenGLShaderProgram *shader) {

  getVisualBall()->init(shader);

  getVisualField()->init(shader);
}
void VisualizedSimWorld::resetBall() {
  ball = std::make_unique<VisualizedSimBall>(dynamicsWorld, worldSettings);
}
void VisualizedSimWorld::resetBall(btVector3 initialPos, btVector3 initialVel) {
  ball = std::make_unique<VisualizedSimBall>(dynamicsWorld, worldSettings,initialPos,initialVel);

}
void VisualizedSimWorld::resetField() {
  field = std::make_unique<VisualizedSimField>(dynamicsWorld,worldSettings);
}
VisualizedSimBall *VisualizedSimWorld::getVisualBall() {
  return dynamic_cast<VisualizedSimBall *>(ball.get());
}
VisualizedSimField *VisualizedSimWorld::getVisualField() {
  return dynamic_cast<VisualizedSimField *>(field.get());
}
void VisualizedSimWorld::resetRobot(bool isBlue, int id, btVector3 position) {
  if(isBlue){
    blueBots.push_back(std::move(
        std::make_unique<VisualizedSimBot>(id, dynamicsWorld, blueSettings, worldSettings,
                                 position * worldSettings.scale, position.z())));
  }else{
    yellowBots.push_back(std::move(
        std::make_unique<VisualizedSimBot>(id, dynamicsWorld, yellowSettings, worldSettings,
                                 position * worldSettings.scale, position.z())));
  }

}

