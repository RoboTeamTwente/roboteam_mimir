//
// Created by rolf on 20-02-21.
//

#include "VisualizerCamera.h"
#include <cmath>

void VisualizerCamera::moveUp() {
    cameraPos+=cameraFront*move_speed;
}

void VisualizerCamera::moveDown() {
    cameraPos-=cameraFront*move_speed;
}

void VisualizerCamera::moveLeft() {
    cameraPos-=QVector3D::crossProduct(cameraFront,cameraUp).normalized()*move_speed;
}
void VisualizerCamera::moveRight() {
    cameraPos+=QVector3D::crossProduct(cameraFront,cameraUp).normalized()*move_speed;
}

void VisualizerCamera::adjust_view_direction(float delta_yaw_degrees, float delta_pitch_degrees) {
    yaw-= delta_yaw_degrees;
    pitch-= delta_pitch_degrees;
    //limit pitch so you can't look exactly up or exactly down
    if (pitch>=89.0){
        pitch=89.0;
    }
    if (pitch<=-89.0){
        pitch=-89.0;
    }
    float yawRad=yaw/180.0f*M_PIf32;
    float pitchRad=pitch/180.0f*M_PIf32;
    QVector3D front(cosf(yawRad)*cosf(pitchRad),sinf(yawRad)*cosf(pitchRad),sinf(pitchRad));
    cameraFront=front.normalized();
}

void VisualizerCamera::window_resize(int w, int h) {
    aspect = float(w) / float(h);
    projection.setToIdentity();
    projection.perspective(viewAngle,aspect,near_plane,far_plane);

}

const QMatrix4x4 &VisualizerCamera::getProjectionMatrix() {
    return projection;
}

QMatrix4x4 VisualizerCamera::getViewMatrix() {
    QMatrix4x4 view;
    view.lookAt(cameraPos,cameraPos+cameraFront,cameraUp);
    return view;
}

void VisualizerCamera::zoom(float zoomin) {

    if(viewAngle>=minAngle&&viewAngle<=maxAngle){
        viewAngle-= zoomin;
    }
    if(viewAngle<minAngle){
        viewAngle=minAngle;
    }
    if(viewAngle>maxAngle){
        viewAngle=maxAngle;
    }
    projection.setToIdentity();
    projection.perspective(viewAngle, aspect, near_plane, far_plane);

}
QVector3D VisualizerCamera::getViewPosition() {
  return cameraPos;
}
