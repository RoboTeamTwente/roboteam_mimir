//
// Created by rolf on 25-11-19.
//

#include "Camera.h"
#include "CollisionShared.h"
#include "iostream"
Camera::Camera(btVector3 camPos, double cx, double cy, double length, double width, btDynamicsWorld *world) :
dynamicsWorld(world),
position(camPos){
    visibleArea=QRectF(cx-length*0.5,cy-width*0.5,length,width);
    std::cout<<visibleArea.topLeft().x()<<" "<<visibleArea.topLeft().y()<<" "
            <<visibleArea.topRight().x()<<" "<<visibleArea.topRight().y()<<" "
            <<visibleArea.bottomRight().x()<<" "<<visibleArea.bottomRight().y()<<" "
            <<visibleArea.bottomLeft().x()<<" "<<visibleArea.bottomLeft().y();
}
bool Camera::isVisible(double x, double y) {
    return visibleArea.contains(x,y);
}

bool Camera::isBallVisible(btVector3 ballPos) {
    if (isVisible(ballPos.x(),ballPos.y())){
        btCollisionWorld::AllHitsRayResultCallback ray(position,ballPos);
        ray.m_collisionFilterGroup = COL_CAMERARAY;
        //ray.m_collisionFilterMask= COL_ROBOT|COL_FIELD;
        // we only want to check for collisions with robots and field elements
        dynamicsWorld->rayTest(position,ballPos,ray);
        if (ray.hasHit()){
            std::cout<<"Vision blocked by bot or field!"<<std::endl; //TODO: remove prints
        }
        else{
            std::cout<<"No vision block!"<<std::endl;
        }
        return !ray.hasHit();
    }
    return false;
}