//
// Created by rolf on 13-10-19.
//

#include "RobotMesh.h"

RobotMesh::RobotMesh(const std::unique_ptr<RobotSettings>& settings) {

    const float halfHeight=(settings->totalHeight-settings->bottomPlateHeight)*0.5f;
    const float radius=settings->radius;
    const float startAngleRad=settings->startAngle/180.0f*M_PI+M_PI_2;
    const float endAngleRad=settings->endAngle/180.0f*M_PI+M_PI_2;
    const float halfFrontLength=sin(startAngleRad)*radius; //length of the front end.
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
