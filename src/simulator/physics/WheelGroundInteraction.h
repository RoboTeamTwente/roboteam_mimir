//
// Created by rolf on 24-11-19.
//

#ifndef ROBOTEAM_MIMIR_WHEELGROUNDINTERACTION_H
#define ROBOTEAM_MIMIR_WHEELGROUNDINTERACTION_H
#include <btBulletDynamicsCommon.h>

enum  bodyType{
    WHEEL=1,
    GROUND=2
};
void customNearCallback(btBroadphasePair &collisionPair, btCollisionDispatcher &dispatcher,
                        const btDispatcherInfo &dispatchInfo);

#endif //ROBOTEAM_MIMIR_WHEELGROUNDINTERACTION_H
