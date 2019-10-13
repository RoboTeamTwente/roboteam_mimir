//
// Created by rolf on 13-10-19.
//

#ifndef ROBOTEAM_MIMIR_ROBOTMESH_H
#define ROBOTEAM_MIMIR_ROBOTMESH_H

#include <QList>
#include "bullet/btBulletDynamicsCommon.h"
#include "../config/RobotSettings.h"

class RobotMesh {
public:
    explicit RobotMesh(RobotSettings* settings);
    const QList<btVector3> &hull() const;
private:

private:
    QList<btVector3> mesh;

};


#endif //ROBOTEAM_MIMIR_ROBOTMESH_H