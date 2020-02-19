//
// Created by rolf on 25-11-19.
//

#ifndef ROBOTEAM_MIMIR_CAMERA_H
#define ROBOTEAM_MIMIR_CAMERA_H

#include <QRectF>
#include <proto/messages_robocup_ssl_geometry.pb.h>
#include "btBulletDynamicsCommon.h"
#include "../config/WorldSettings.h"

class Camera {
public:
    Camera(CameraSettings settings, btDynamicsWorld *world, double worldScale);
    [[nodiscard]] bool isBallVisible(btVector3 ballPos) const;
    [[nodiscard]] bool isVisible(btVector3 point) const;
    [[nodiscard]] bool isInImage(double x, double y) const;
    [[nodiscard]] int getId() const;
    [[nodiscard]] btVector3 fieldToImage(btVector3 fieldPoint) const;
    [[nodiscard]] btVector3 imageToField(btVector3 imagePoint, double assumedHeight) const;
    [[nodiscard]] SSL_GeometryCameraCalibration asMessage() const;
    [[nodiscard]] btVector3 extrapolation(btVector3 ballPoint, double assumedHeight) const;

private:
    double scaleToCamera() const;
    double scaleFromCamera() const;
    int id = 0;
    btVector3 position;
    btDynamicsWorld *dynamicsWorld;
    double focalLength;
    double principalPointX;
    double principalPointY;
    double distortion;
    double scale;
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
