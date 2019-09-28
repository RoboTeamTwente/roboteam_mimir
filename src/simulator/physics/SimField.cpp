//
// Created by rolf on 28-09-19.
//

#include "SimField.h"

SimField::SimField(btDynamicsWorld *world) {
    dynamicsWorld=world;
    ground=new btStaticPlaneShape(btVector3(0.0f,0.0f,1.0f),0.0f);//creates the floor
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(0.0f,0.0f,0.0f));
    addObject(ground,transform);
}
SimField::~SimField() {
    for (int i = 0; i < objects.size(); ++i) {
        delete objects[i];
    }
    delete ground;
}
void SimField::addObject(btCollisionShape *shape, const btTransform &transform) {
    //create a new object for collisions
    btCollisionObject* object=new btCollisionObject;
    object->setCollisionShape(shape);
    // put it into the world
    object->setWorldTransform(transform);

    object->setRestitution(0.0);
    object->setFriction(0.0);
    object->setRollingFriction(0.0);
    dynamicsWorld->addCollisionObject(object);
    objects.push_back(object);

}