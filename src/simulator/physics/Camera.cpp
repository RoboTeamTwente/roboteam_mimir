//
// Created by rolf on 25-11-19.
//

#include "Camera.h"
#include "CollisionShared.h"

Camera::Camera(CameraSettings settings, btDynamicsWorld *world, double worldScale) {
    dynamicsWorld = world;
    distortion = settings.distortion;
    principalPointX = settings.principalPointX;
    principalPointY = settings.principalPointY;
    focalLength = settings.focalLength;

    q = btQuaternion(settings.q0,
                     settings.q1,
                     settings.q2,
                     settings.q3);
    t = btVector3(settings.tx, settings.ty, settings.tz);
    position = btVector3(settings.derivedTX, settings.derivedTY, settings.derivedTZ);
    id = settings.cameraID;
    resolutionX = settings.camResolutionX;
    resolutionY = settings.camResolutionY;
    scale=worldScale;
}

bool Camera::isBallVisible(btVector3 ballPos) const {
    //Raytests need to be done in world coordinates
    btCollisionWorld::AllHitsRayResultCallback ray(position*scaleFromCamera(), ballPos);
    ray.m_collisionFilterGroup = COL_CAMERARAY;
    ray.m_collisionFilterMask= COL_ROBOT|COL_FIELD; //TODO: fix or remove
    // we only want to check for collisions with robots and field elements
    dynamicsWorld->rayTest(position*scaleFromCamera(), ballPos, ray);
    return !ray.hasHit();

}
int Camera::getId() const {
    return id;
}

btVector3 Camera::fieldToImage(btVector3 fieldPoint) const {
    fieldPoint*=scaleToCamera();
    btQuaternion fieldToCam = q.normalized();
    btVector3 translation = t;

    // First transform the point from the field into the coordinate system of the
    // camera
    btVector3 p_c = rotateVectorByQuaternion(fieldPoint, fieldToCam) + translation;
    btVector3 p_un = btVector3(p_c.x() / p_c.z(), p_c.y() / p_c.z(),
                               0);// we are representing 2d vecs with 3d vecs here!

    // Apply distortion
    btVector3 pd = radialDistortion(p_un);
    // Then project from the camera coordinate system onto the image plane using
    // the instrinsic parameters
    btVector3 p_i = focalLength * pd + btVector3(principalPointX, principalPointY, 0);
    return p_i;
}
//AGAIN btVector3 used as both 2d/3d vector! be warned!
// Assumed height is in WORLD coordinates
// This returns in WORLD coordinates (not camera coordinates!)
btVector3 Camera::imageToField(btVector3 imagePoint, double assumedHeight) const {
    //undo scaling and offset
    btVector3 p_d = btVector3((imagePoint.x() - principalPointX) / focalLength,
                              (imagePoint.y() - principalPointY) / focalLength, 0);
    // Compensate for distortion (undistort)
    btVector3 p_un = radialDistortionInv(p_d);
    // Now we got a ray on the z axis
    p_un.setZ(1.0);
    btVector3 v = p_un;

    // Transform this ray into world coordinates
    btQuaternion fieldToCam = q.normalized();
    btVector3 translation = t;
    btQuaternion fieldToCamInv = fieldToCam.inverse();
    btVector3 v_in_w = rotateVectorByQuaternion(v, fieldToCamInv);
    btVector3 zero_in_w = rotateVectorByQuaternion(-translation, fieldToCamInv);
    double t = rayPlaneIntersect(btVector3(0, 0, assumedHeight*scaleToCamera()), btVector3(0, 0, 1), zero_in_w, v_in_w.normalized());
    btVector3 p_f = zero_in_w + v_in_w.normalized() * t;
    return p_f*scaleFromCamera();
}
btVector3 Camera::rotateVectorByQuaternion(btVector3 vec, btQuaternion quat) {
    btScalar x2 = quat.x() * quat.x();
    btScalar y2 = quat.y() * quat.y();
    btScalar z2 = quat.z() * quat.z();
    btScalar xy = quat.x() * quat.y();
    btScalar xz = quat.x() * quat.z();
    btScalar yz = quat.y() * quat.z();
    btScalar wx = quat.w() * quat.x();
    btScalar wy = quat.w() * quat.y();
    btScalar wz = quat.w() * quat.z();
    btVector3 result;
    result.setX((1.0 - 2.0 * (y2 + z2)) * vec.x() + (2.0 * (xy - wz)) * vec.y() + (2.0 * (xz + wy)) * vec.z());
    result.setY((2.0 * (xy + wz)) * vec.x() + (1.0 - 2.0 * (x2 + z2)) * vec.y() + (2.0 * (yz - wx)) * vec.z());
    result.setZ((2.0 * (xz - wy)) * vec.x() + (2.0 * (yz + wx)) * vec.y() + (1.0 - 2.0 * (x2 + y2)) * vec.z());
    return result;
}

//NOTE THE 3D vector here is really a 2d one!!!
btVector3 Camera::radialDistortion(btVector3 pu) const {
    double rd = radialDistortion(pu.length());
    btVector3 pd = pu.normalized() * rd;
    return pd;
}

double Camera::radialDistortion(double ru) const{
    if (distortion <= DBL_MIN)
        return ru;
    double rd = 0;
    double a = distortion;
    double b = -9.0 * a * a * ru + a * sqrt(a * (12.0 + 81.0 * a * ru * ru));
    b = (b < 0.0) ? (-pow(b, 1.0 / 3.0)) : pow(b, 1.0 / 3.0);
    rd = pow(2.0 / 3.0, 1.0 / 3.0) / b -
         b / (pow(2.0 * 3.0 * 3.0, 1.0 / 3.0) * a);
    return rd;
}

double Camera::radialDistortionInv(double rd) const{
    return rd * (1.0 + rd * rd * distortion);
}
btVector3 Camera::radialDistortionInv(btVector3 pd) const {
    double ru = radialDistortionInv(pd.length());
    btVector3 pu = pd.normalized() * ru;
    return pu;
}
bool Camera::isInImage(double x, double y) const {
    return x > 0 && x < resolutionX && y > 0 && y < resolutionY;
}
//take care, quite expensive!
bool Camera::isVisible(btVector3 point) const {
    btVector3 imagePoint=fieldToImage(point);
    return isInImage(imagePoint.x(),imagePoint.y());
}
double Camera::scaleToCamera() const {
    return 1000.0/scale;
}
double Camera::scaleFromCamera() const {
    return scale/1000.0;
}