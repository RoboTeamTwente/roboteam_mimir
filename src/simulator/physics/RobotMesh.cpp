//
// Created by rolf on 13-10-19.
//

#include "RobotMesh.h"

RobotMesh::RobotMesh(RobotSettings *settings) {

    const float halfHeight=settings->height*0.5f;
    const float radius=settings->radius;
    const float startAngleRad=settings->startAngle/180.0f*M_PI;
    const float endAngleRad=settings->endAngle/180.0f*M_PI;
    const float halfFrontLength=sin(startAngleRad)*radius;
    //first construct the triangle mesh
    const int angleSteps=20;
    const float angleStep=(endAngleRad-startAngleRad)/angleSteps;
    float angle=startAngleRad;

    //we construct top and bottom alternating to ensure valid triangle mesh. Not sure if there is a better/easier way.
    //TODO: figure out if collision margin matters much here
    for (int i = 0; i < angleSteps; ++i) {
        mesh.append(btVector3(radius*sin(angle),radius*cos(angle),halfHeight));
        mesh.append(btVector3(radius*sin(angle),radius*cos(angle),-halfHeight));
        angle+=angleStep;
    }
    //also append the front side still
    mesh.append(btVector3(radius*sin(endAngleRad),radius*cos(endAngleRad),halfHeight));
    mesh.append(btVector3(radius*sin(endAngleRad),radius*cos(endAngleRad),-halfHeight));


}

const QList<btVector3>& RobotMesh::hull() const {
    return mesh;
}