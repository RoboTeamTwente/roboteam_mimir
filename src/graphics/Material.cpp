//
// Created by rolf on 10-03-21.
//

#include "Material.h"

Material::Material(QVector3D color, float shininess) :
ambient{color},
diffuse{color},
specular{color},
shininess{shininess}{ }
 Material Material::black_plastic() {
  return Material(QVector3D(),QVector3D(0.01,0.01,0.01),
                  QVector3D(0.5,0.5,0.5),32);
}

Material::Material(QVector3D ambient, QVector3D diffuse, QVector3D specular, float shininess) :
ambient{ambient},
diffuse{diffuse},
specular{specular},
shininess{shininess}{
}
