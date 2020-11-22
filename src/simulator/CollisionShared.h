//
// Created by rolf on 24-11-19.
//

#ifndef ROBOTEAM_MIMIR_COLLISIONSHARED_H
#define ROBOTEAM_MIMIR_COLLISIONSHARED_H
#include <btBulletDynamicsCommon.h>

enum  bodyType{
    WHEEL=1,
    GROUND=2
};
#define BIT(x) (1<<(x))
enum collisionType{
    COL_NOTHING=0,
    COL_FIELD=BIT(1),
    COL_BALL=BIT(2),
    COL_ROBOT=BIT(3),
    COL_CAMERARAY=BIT(4),
    COL_EVERYTHING= COL_FIELD|COL_BALL|COL_ROBOT|COL_CAMERARAY
};
void customNearCallback(btBroadphasePair &collisionPair, btCollisionDispatcher &dispatcher,
                        const btDispatcherInfo &dispatchInfo);

#endif //ROBOTEAM_MIMIR_COLLISIONSHARED_H
