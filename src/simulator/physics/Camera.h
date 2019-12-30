//
// Created by rolf on 25-11-19.
//

#ifndef ROBOTEAM_MIMIR_CAMERA_H
#define ROBOTEAM_MIMIR_CAMERA_H

#include <QRectF>
#include "btBulletDynamicsCommon.h"
#include "../config/WorldConfig.h"

class Camera {
public:
    Camera(CameraSettings settings, btDynamicsWorld *world, double worldScale);
    bool isBallVisible(btVector3 ballPos) const;
    bool isVisible(btVector3 point) const;
    bool isInImage(double x, double y) const;
    int getId() const;
    btVector3 fieldToImage(btVector3 fieldPoint) const;
    btVector3 imageToField(btVector3 imagePoint, double assumedHeight) const;

private:

    int id = 0;
    btVector3 position;
    btDynamicsWorld *dynamicsWorld;
    double focalLength;
    double principalPointX;
    double principalPointY;
    double distortion;
    btVector3 t;
    btQuaternion q;
    int resolutionX;
    int resolutionY;

    //The below functions are defined as in SSL-vision so I use similar naming where possible
    static btVector3 rotateVectorByQuaternion(btVector3 vec, btQuaternion quat);
    btVector3 radialDistortionInv(btVector3 pd) const;
    double radialDistortionInv(double rd) const;
    btVector3 radialDistortion(btVector3 pu) const;
    double radialDistortion(double ru) const;
    static double rayPlaneIntersect(btVector3 pOrigin, btVector3 pNormal, btVector3 rOrigin, btVector3 rVector) {
        return (-pNormal).dot(rOrigin - pOrigin) / (pNormal.dot(rVector));
    }

};

#endif //ROBOTEAM_MIMIR_CAMERA_H
