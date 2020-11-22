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
    [[nodiscard]] const std::vector<btVector3> &hull() const;
private:
    std::vector<btVector3> mesh;

};


#endif //ROBOTEAM_MIMIR_ROBOTMESH_H
