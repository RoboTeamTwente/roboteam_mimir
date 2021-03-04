//
// Created by rolf on 28-09-19.
//

#ifndef ROBOTEAM_MIMIR_SIMFIELD_H
#define ROBOTEAM_MIMIR_SIMFIELD_H

#include <btBulletDynamicsCommon.h>
#include "settings/WorldSettings.h"
#include <memory>
#include <BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.h>

class SimField {
public:
    explicit SimField(std::shared_ptr<btMultiBodyDynamicsWorld> world, const WorldSettings& cfg);
    virtual ~SimField();
 protected:
  static std::vector<btTransform> goalBackTransforms(const WorldSettings &cfg);
  static std::vector<btTransform> goalSideTransforms(const WorldSettings &cfg);
  static btVector3 goalBackDimensions(const WorldSettings &cfg);
  static btVector3 goalSideDimensions(const WorldSettings &cfg);

  std::shared_ptr<btMultiBodyDynamicsWorld> dynamicsWorld;

    btAlignedObjectArray<btCollisionObject*> objects;

    btStaticPlaneShape * plane;

    btCollisionShape * goalBack;
    btCollisionShape * goalSide;
    void addObject(btCollisionShape *shape, const btTransform &transform);

};


#endif //ROBOTEAM_MIMIR_SIMFIELD_H
