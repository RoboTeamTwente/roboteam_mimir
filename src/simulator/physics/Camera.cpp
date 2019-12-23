//
// Created by rolf on 25-11-19.
//

#include "Camera.h"
#include "CollisionShared.h"
#include "iostream"
Camera::Camera(btVector3 camPos, double cx, double cy, double length, double width, btDynamicsWorld* world)
        :
        dynamicsWorld(world),
        position(camPos) {
    visibleArea = QRectF(cx - length*0.5, cy - width*0.5, length, width);
    std::cout << visibleArea.topLeft().x() << " " << visibleArea.topLeft().y() << " "
              << visibleArea.topRight().x() << " " << visibleArea.topRight().y() << " "
              << visibleArea.bottomRight().x() << " " << visibleArea.bottomRight().y() << " "
              << visibleArea.bottomLeft().x() << " " << visibleArea.bottomLeft().y();
}
bool Camera::isVisible(double x, double y) const {
    return visibleArea.contains(x, y);
}

bool Camera::isBallVisible(btVector3 ballPos) const {
    if (isVisible(ballPos.x(), ballPos.y())) {
        btCollisionWorld::AllHitsRayResultCallback ray(position, ballPos);
        ray.m_collisionFilterGroup = COL_CAMERARAY;
        //ray.m_collisionFilterMask= COL_ROBOT|COL_FIELD;
        // we only want to check for collisions with robots and field elements
        dynamicsWorld->rayTest(position, ballPos, ray);
        return ! ray.hasHit();
    }
    return false;
}
int Camera::getId() const {
    return id;
}

btVector3 Camera::fieldToImage(btVector3 fieldPoint) {
    btScalar q0 = 0.700049, q1 = - 0.713937, q2 = 0.003163, q3 = - 0.01468, tx = - 838.942078, ty = 3717.62769, tz = 6090, focal_length = 1361.73291, principal_point_x = 1411.21851, principal_point_y = 818.82782;
    btQuaternion fieldToCam = btQuaternion(q0, q1, q2, q3).normalized();
    btVector3 translation = btVector3(tx, ty, tz);

    // First transform the point from the field into the coordinate system of the
    // camera
    btVector3 p_c = rotateVectorByQuaternion(fieldPoint, fieldToCam) + translation;
    btVector3 p_un = btVector3(p_c.x()/p_c.z(), p_c.y()/p_c.z(), 0);// we are representing 2d vecs with 3d vecs here!

    // Apply distortion
    btVector3 pd = radialDistortion(p_un);
    // Then project from the camera coordinate system onto the image plane using
    // the instrinsic parameters
    btVector3 p_i = focal_length*pd + btVector3(principal_point_x, principal_point_y, 0);
    return p_i;
}
//AGAIN btVector3 used as both 2d/3d vector! be warned!
btVector3 Camera::imageToField(btVector3 imagePoint, double assumedHeight) {
    btScalar q0 = 0.700049, q1 = - 0.713937, q2 = 0.003163, q3 = - 0.01468, tx = - 838.942078, ty = 3717.62769, tz = 6090, focal_length = 1361.73291, principal_point_x = 1411.21851, principal_point_y = 818.82782;
    //undo scaling and offset
    btVector3 p_d = btVector3((imagePoint.x() - principal_point_x)/focal_length,
            (imagePoint.y() - principal_point_y)/focal_length, 0);
    // Compensate for distortion (undistort)
    btVector3 p_un = radialDistortionInv(p_d);
    // Now we got a ray on the z axis
    p_un.setZ(1.0);
    btVector3 v = p_un;

    // Transform this ray into world coordinates
    btQuaternion fieldToCam = btQuaternion(q0, q1, q2, q3).normalized();
    btVector3 translation = btVector3(tx, ty, tz);
    btQuaternion fieldToCamInv = fieldToCam.inverse();
    btVector3 v_in_w = rotateVectorByQuaternion(v, fieldToCamInv);
    btVector3 zero_in_w = rotateVectorByQuaternion(- translation, fieldToCamInv);
    double t = rayPlaneIntersect(btVector3(0, 0, assumedHeight), btVector3(0, 0, 1), zero_in_w, v_in_w.normalized());
    btVector3 p_f = zero_in_w + v_in_w.normalized()*t;
    return p_f;
}
btVector3 Camera::rotateVectorByQuaternion(btVector3 vec, btQuaternion quat) {
    btScalar x2 = quat.x()*quat.x();
    btScalar y2 = quat.y()*quat.y();
    btScalar z2 = quat.z()*quat.z();
    btScalar xy = quat.x()*quat.y();
    btScalar xz = quat.x()*quat.z();
    btScalar yz = quat.y()*quat.z();
    btScalar wx = quat.w()*quat.x();
    btScalar wy = quat.w()*quat.y();
    btScalar wz = quat.w()*quat.z();
    btVector3 result;
    result.setX((1.0 - 2.0*(y2 + z2))*vec.x() + (2.0*(xy - wz))*vec.y() + (2.0*(xz + wy))*vec.z());
    result.setY((2.0*(xy + wz))*vec.x() + (1.0 - 2.0*(x2 + z2))*vec.y() + (2.0*(yz - wx))*vec.z());
    result.setZ((2.0*(xz - wy))*vec.x() + (2.0*(yz + wx))*vec.y() + (1.0 - 2.0*(x2 + y2))*vec.z());
    return result;
}

//NOTE THE 3D vector here is really a 2d one!!!
btVector3 Camera::radialDistortion(btVector3 pu) {
    double rd = radialDistortion(pu.length());
    btVector3 pd = pu.normalized()*rd;
    return pd;
}

double Camera::radialDistortion(double ru) {
    double dist = - 0.003282;
    if (dist <= DBL_MIN)
        return ru;
    double rd = 0;
    double a = dist;
    double b = - 9.0*a*a*ru + a*sqrt(a*(12.0 + 81.0*a*ru*ru));
    b = (b < 0.0) ? (- pow(b, 1.0/3.0)) : pow(b, 1.0/3.0);
    rd = pow(2.0/3.0, 1.0/3.0)/b -
            b/(pow(2.0*3.0*3.0, 1.0/3.0)*a);
    return rd;
}

double Camera::radialDistortionInv(double rd) {
    double dist = - 0.003282;
    return rd*(1.0 + rd*rd*dist);
}
btVector3 Camera::radialDistortionInv(btVector3 pd) {
    double ru = radialDistortionInv(pd.length());
    btVector3 pu = pd.normalized()*ru;
    return pu;
}