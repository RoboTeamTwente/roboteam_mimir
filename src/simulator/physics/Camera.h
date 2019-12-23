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
        Camera(btVector3 camPos, double cx, double cy, double length, double width, btDynamicsWorld* world);
        bool isVisible(double x, double y) const;
        bool isBallVisible(btVector3 ballPos) const;
        int getId() const;
        btVector3 fieldToImage(btVector3 fieldPoint);
        btVector3 imageToField(btVector3 imagePoint, double assumedHeight);

    private:

        int id = 0;//TODO: fix ID
        btVector3 position;
        QRectF visibleArea;
        btDynamicsWorld* dynamicsWorld;
        //TODO: properly use and send through geometry
        //The below functions are defined as in SSL-vision so I use similar naming where possible
        static btVector3 rotateVectorByQuaternion(btVector3 vec, btQuaternion quat);
        btVector3 radialDistortionInv(btVector3 pd);
        double radialDistortionInv(double rd);
        btVector3 radialDistortion(btVector3 pu);
        double radialDistortion(double ru);
        static double rayPlaneIntersect(btVector3 pOrigin, btVector3 pNormal, btVector3 rOrigin, btVector3 rVector){
            return (-pNormal).dot(rOrigin-pOrigin)/ (pNormal.dot(rVector));
        }

        //    double focalLength;
//    double principalPointX;
//    double principalPointY;
//    double distortion;
//    btVector3 t;
//    btQuaternion q;

};

#endif //ROBOTEAM_MIMIR_CAMERA_H
