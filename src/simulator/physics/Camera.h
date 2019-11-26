//
// Created by rolf on 25-11-19.
//

#ifndef ROBOTEAM_MIMIR_CAMERA_H
#define ROBOTEAM_MIMIR_CAMERA_H

#include <QRectF>
#include "btBulletDynamicsCommon.h"

//TODO: add pinhole model for camera (with distortion later possibly)
class Camera {
public:
    Camera(btVector3 camPos,double cx, double cy, double length, double width,btDynamicsWorld* world);
    bool isVisible(double x, double y);
    bool isBallVisible(btVector3 ballPos);
private:
    //We use QTvectors/quaternions/matrices here, not bullet, since QT has a lot more convenient camera operations with Quaternions and Vectors
    //which are lacking in Bullet.
    btVector3 position;
    QRectF visibleArea;
    btDynamicsWorld* dynamicsWorld;
    //TODO: properly use and send through geometry
//    double focalLength;
//    double principalPointX;
//    double principalPointY;
//    double distortion;
//    btVector3 t;
//    btQuaternion q;

};


#endif //ROBOTEAM_MIMIR_CAMERA_H
