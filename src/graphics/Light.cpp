//
// Created by rolf on 10-03-21.
//

#include "Light.h"

PointLight::PointLight(QVector3D position, QVector3D color) : position{position},ambient{color},diffuse{color},specular{color}{};
