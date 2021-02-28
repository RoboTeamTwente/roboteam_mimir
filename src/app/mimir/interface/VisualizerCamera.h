//
// Created by rolf on 20-02-21.
//

#ifndef ROBOTEAM_MIMIR_VISUALIZERCAMERA_H
#define ROBOTEAM_MIMIR_VISUALIZERCAMERA_H

#include <QMatrix4x4>

class VisualizerCamera {
public:
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void adjust_view_direction(float delta_yaw_degrees, float delta_pitch_degrees);
    void window_resize(int width, int height);
    void zoom(float zoomin);
    QMatrix4x4 getViewMatrix();
    const QMatrix4x4& getProjectionMatrix();
private:
    //movement speed
    const float move_speed = 0.1;

    //viewAngle settings and zoom
    const float minAngle=1.0;
    const float maxAngle=60.0;

    float near_plane = 0.1;
    float far_plane = 100.0;
    float viewAngle=60.0;
    float aspect=4.0/3.0;
    QMatrix4x4 projection;
    float yaw=90.0;
    float pitch=-45.0;
    QVector3D cameraPos={0.0,-4.5,4.5};
    QVector3D cameraFront=QVector3D(0.0,0.7071f,-0.7071f).normalized();
    QVector3D cameraUp={0.0,0.0,1.0};
};


#endif //ROBOTEAM_MIMIR_VISUALIZERCAMERA_H
