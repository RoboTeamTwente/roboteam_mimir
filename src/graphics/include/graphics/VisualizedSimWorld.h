//
// Created by rolf on 23-11-20.
//

#ifndef ROBOTEAM_MIMIR_SRC_GRAPHICS_VISUALIZEDSIMWORLD_H_
#define ROBOTEAM_MIMIR_SRC_GRAPHICS_VISUALIZEDSIMWORLD_H_

#include <simulator/SimWorld.h>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class VisualizedSimBall;
class VisualizedSimField;
class VisualizedSimBot;

class VisualizedSimWorld : public SimWorld {
 public:
  VisualizedSimWorld(const WorldSettings& world_settings, const RobotSettings&blue,
                     const RobotSettings& yellow, const SituationWorld& situation);
  void init_gl(QOpenGLShaderProgram * shader);
  void draw(QOpenGLShaderProgram *shader,QOpenGLFunctions* gl);
 protected:
  void resetBall() override;
  void resetBall(btVector3 initialPos, btVector3 initialVel) override;
  void resetField() override;
  void addRobot(bool isBlue, unsigned int id, btVector3 position) override;

 private:
  VisualizedSimBall * getVisualBall();
  VisualizedSimField * getVisualField();
  VisualizedSimBot * getRobot(const std::unique_ptr<SimBot>& robot);
};

#endif //ROBOTEAM_MIMIR_SRC_GRAPHICS_VISUALIZEDSIMWORLD_H_
