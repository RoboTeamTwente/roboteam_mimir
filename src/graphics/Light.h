//
// Created by rolf on 10-03-21.
//

#ifndef ROBOTEAM_MIMIR_SRC_GRAPHICS_LIGHT_H_
#define ROBOTEAM_MIMIR_SRC_GRAPHICS_LIGHT_H_
#include <QVector3D>
struct PointLight{
  PointLight(QVector3D position, QVector3D color);
  QVector3D position;
  QVector3D ambient;
  QVector3D diffuse;
  QVector3D specular;
};

#endif //ROBOTEAM_MIMIR_SRC_GRAPHICS_LIGHT_H_
