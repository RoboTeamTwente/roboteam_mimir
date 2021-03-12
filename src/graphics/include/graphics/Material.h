//
// Created by rolf on 10-03-21.
//

#ifndef ROBOTEAM_MIMIR_SRC_GRAPHICS_MATERIAL_H_
#define ROBOTEAM_MIMIR_SRC_GRAPHICS_MATERIAL_H_

#include <QVector3D>
#include <QOpenGLShaderProgram>

struct Material {
  Material()=default;
  Material(QVector3D color, float shininess);
  Material(QVector3D ambient, QVector3D diffuse, QVector3D specular, float shininess);
  void bind(QOpenGLShaderProgram * shader) const;
  static Material black_plastic();
  QVector3D ambient;
  QVector3D diffuse;
  QVector3D specular;
  float shininess= 0.0;
};

#endif //ROBOTEAM_MIMIR_SRC_GRAPHICS_MATERIAL_H_
