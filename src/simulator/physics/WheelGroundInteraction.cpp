//
// Created by rolf on 24-11-19.
//

#include "WheelGroundInteraction.h"
#include <iostream> //TODO: remove redundant include

bool isWheelGroundCollision(btCollisionObject *obj0, btCollisionObject *obj1) {
    return (obj0->getUserIndex() == bodyType::WHEEL &&
            obj1->getUserIndex() == bodyType::GROUND) ||
            (obj1->getUserIndex() == bodyType::WHEEL &&
            obj0->getUserIndex() == bodyType::GROUND);
}
void editContactPoint(btManifoldPoint &point){
    //TODO: fix control
    point.m_contactCFM=0.01; //TODO: tune this value
}
//This callback is almost identical to btCollisionDispatcher:defaultNearCallBack, except we check and edit some collision points
void customNearCallback(btBroadphasePair &collisionPair, btCollisionDispatcher &dispatcher,
                        const btDispatcherInfo &dispatchInfo) {
    btCollisionObject *colObj0 = (btCollisionObject *) collisionPair.m_pProxy0->m_clientObject;
    btCollisionObject *colObj1 = (btCollisionObject *) collisionPair.m_pProxy1->m_clientObject;

    if (dispatcher.needsCollision(colObj0, colObj1)) {
        btCollisionObjectWrapper obj0Wrap(0, colObj0->getCollisionShape(), colObj0, colObj0->getWorldTransform(), -1,
                                          -1);
        btCollisionObjectWrapper obj1Wrap(0, colObj1->getCollisionShape(), colObj1, colObj1->getWorldTransform(), -1,
                                          -1);

        //dispatcher will keep algorithms persistent in the collision pair
        if (!collisionPair.m_algorithm) {
            collisionPair.m_algorithm = dispatcher.findAlgorithm(&obj0Wrap, &obj1Wrap, 0, BT_CONTACT_POINT_ALGORITHMS);
        }

        if (collisionPair.m_algorithm) {
            btManifoldResult contactPointResult(&obj0Wrap, &obj1Wrap);

            if (dispatchInfo.m_dispatchFunc == btDispatcherInfo::DISPATCH_DISCRETE) {
                //discrete collision detection query

                collisionPair.m_algorithm->processCollision(&obj0Wrap, &obj1Wrap, dispatchInfo, &contactPointResult);
                //START OF EDITED CODE FOR MIMIR
                if (isWheelGroundCollision(colObj0,colObj1)){
                    if (contactPointResult.getPersistentManifold()){
                        int numContacts = contactPointResult.getPersistentManifold()->getNumContacts();
                        for (int i = 0; i < numContacts; ++i) {
                            editContactPoint(contactPointResult.getPersistentManifold()->getContactPoint(i));
                        }
                    }
                }
                //END MIMIR Code
            } else {
                //continuous collision detection query, time of impact (toi)
                btScalar toi = collisionPair.m_algorithm->calculateTimeOfImpact(colObj0, colObj1, dispatchInfo,
                                                                                &contactPointResult);
                if (dispatchInfo.m_timeOfImpact > toi)
                    dispatchInfo.m_timeOfImpact = toi;
            }
        }
    }
}
