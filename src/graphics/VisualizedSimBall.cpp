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
  visual->draw(shader,gl,mat);
}
VisualizedSimBall::VisualizedSimBall(std::shared_ptr<btMultiBodyDynamicsWorld> world,
                                     const WorldSettings &settings,
                                     btVector3 initial_pos,
                                     btVector3 initial_vel) :
    SimBall(std::move(world),settings,initial_pos,initial_vel){
  visual= std::make_unique<VisualizerBall>(settings.ballRadius,1.0,165.0/255.0,0);
}
VisualizedSimBall::~VisualizedSimBall() {

}
