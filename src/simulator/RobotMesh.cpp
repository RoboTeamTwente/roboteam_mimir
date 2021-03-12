//
// Created by rolf on 13-10-19.
//

#include "RobotMesh.h"

RobotMesh::RobotMesh(const RobotSettings& settings) {

    const float halfHeight=(settings.totalHeight-settings.bottomPlateHeight)*0.5f;
    const float radius=settings.radius;
    const float startAngleRad=settings.startAngle/180.0f*M_PI+M_PI_2;
    const float endAngleRad=settings.endAngle/180.0f*M_PI+M_PI_2;
    const float halfFrontLength=sin(startAngleRad)*radius; //length of the front end.
    //first construct the triangle mesh
    const int angleSteps=20;
    const float angleStep=(endAngleRad-startAngleRad)/angleSteps;
    float angle=startAngleRad;

    topCenter = btVector3(0,0,halfHeight);
    bottomCenter = btVector3(0,0,-halfHeight);
    //we construct top and bottom alternating to ensure valid triangle mesh. Not sure if there is a better/easier way.
    //TODO: figure out if collision margin matters much here
    for (int i = 0; i < angleSteps; ++i) {
        top_points.emplace_back(radius*sin(angle),radius*cos(angle),halfHeight);
        bottom_points.emplace_back(radius*sin(angle),radius*cos(angle),-halfHeight);
        angle+=angleStep;
    }
    //also append the front side still
    top_points.emplace_back(radius*sin(endAngleRad),radius*cos(endAngleRad),halfHeight);
    bottom_points.emplace_back(radius*sin(endAngleRad),radius*cos(endAngleRad),-halfHeight);


}
std::vector<btVector3> RobotMesh::complete_hull() const {
    std::vector<btVector3> out;
    for (int i = 0; i < top_points.size(); ++i) {
      out.emplace_back(top_points[i]);
      out.emplace_back(bottom_points[i]);
    }
    return out;
}
btVector3 RobotMesh::top() const {
  return topCenter;
}
btVector3 RobotMesh::bottom() const {
  return bottomCenter;
}
std::vector<btVector3> RobotMesh::topCircle() const {
  return top_points;
}
std::vector<btVector3> RobotMesh::bottomCircle() const {
  return bottom_points;
}


