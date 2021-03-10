//
// Created by rolf on 16-01-21.
//

#include "VisualizedSimBall.h"
#include "VisualizerUtil.h"

#include <utility>
VisualizedSimBall::VisualizedSimBall(std::shared_ptr<btMultiBodyDynamicsWorld> world, const WorldSettings &settings) :
SimBall(std::move(world),settings){
  visual= std::make_unique<VisualizerBall>(settings.ballRadius,1.0,165.0/255.0,0);
}
void VisualizedSimBall::init(QOpenGLShaderProgram *shader) {
  visual->init(shader);
}
void VisualizedSimBall::draw(QOpenGLShaderProgram *shader, QOpenGLFunctions *gl) {
  btTransform transform = multiBody->getBaseWorldTransform();
  QMatrix4x4 mat = getGLTransform(transform,1.0/SCALE);
  shader->setUniformValue("material.ambient",material.ambient);
  shader->setUniformValue("material.diffuse",material.diffuse);
  shader->setUniformValue("material.specular",material.specular);
  shader->setUniformValue("material.shininess",material.shininess);
  visual->draw(shader,gl,mat);
}
VisualizedSimBall::VisualizedSimBall(std::shared_ptr<btMultiBodyDynamicsWorld> world,
                                     const WorldSettings &settings,
                                     btVector3 initial_pos,
                                     btVector3 initial_vel) :
    SimBall(std::move(world),settings,initial_pos,initial_vel){
  visual= std::make_unique<VisualizerBall>(settings.ballRadius,1.0,165.0/255.0,0);
  material = Material(QVector3D(0.0,0.0,0.0),
                      QVector3D(0.9,0.55,0.0),
                      QVector3D(1.0,0.65,0.5),
                      32.0);
}
VisualizedSimBall::~VisualizedSimBall() {

}
