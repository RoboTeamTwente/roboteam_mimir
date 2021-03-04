//
// Created by rolf on 03-03-21.
//

#ifndef ROBOTEAM_MIMIR_SRC_GRAPHICS_INCLUDE_GRAPHICS_VISUALIZERUTIL_H_
#define ROBOTEAM_MIMIR_SRC_GRAPHICS_INCLUDE_GRAPHICS_VISUALIZERUTIL_H_

#include <QMatrix4x4>
#include <btBulletDynamicsCommon.h>

inline QMatrix4x4 getGLTransform(const btTransform & transform, double scale){
  btTransform scaled_transform;
  scaled_transform.setIdentity();
  scaled_transform.setOrigin(transform.getOrigin()*scale);
  scaled_transform.setRotation(transform.getRotation());

  //convert btTransform to a QMatrix 4x4
  QMatrix4x4 mat;
  btScalar data[16];
  scaled_transform.getOpenGLMatrix(data);
  for (int i = 0; i < 16; ++i) {
    mat.data()[i] = data[i];
  }
  return mat;
}
#endif //ROBOTEAM_MIMIR_SRC_GRAPHICS_INCLUDE_GRAPHICS_VISUALIZERUTIL_H_
