//
// Created by rolf on 13-10-19.
//

#ifndef ROBOTEAM_MIMIR_ROBOTMESH_H
#define ROBOTEAM_MIMIR_ROBOTMESH_H

#include <vector>
#include <btBulletDynamicsCommon.h>
#include "settings/RobotSettings.h"
#include <memory>

class RobotMesh {
public:
    explicit RobotMesh(const RobotSettings& settings);
    btVector3 top() const;
    btVector3 bottom() const;
    std::vector<btVector3> topCircle() const;
    std::vector<btVector3> bottomCircle() const;
    [[nodiscard]] std::vector<btVector3> complete_hull() const;

private:
  btVector3 topCenter;
  std::vector<btVector3> top_points;
  std::vector<btVector3> bottom_points;
  btVector3 bottomCenter;

};


#endif //ROBOTEAM_MIMIR_ROBOTMESH_H
