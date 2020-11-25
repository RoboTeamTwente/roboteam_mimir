//
// Created by rolf on 24-11-19.
//

#include "CollisionShared.h"
#include <iostream> //TODO: remove redundant include
#include <BulletCollision/CollisionDispatch/btManifoldResult.h>
#include <iomanip>

std::unordered_map<CollisionPair,Material,CollisionTypeHasher> MaterialManager::materialMap;

void MaterialManager::initialize() {
  deinitialize();
  gContactAddedCallback = MaterialManager::contactAddedCallback;
}
void MaterialManager::deinitialize() {
  gContactAddedCallback = nullptr;
  materialMap.clear();
}
Material MaterialManager::getMaterial(CollisionType first, CollisionType second) {
  CollisionPair pair{
    .first = first,
    .second = second
  };
  auto it = materialMap.find(pair);
  if(it == materialMap.end()){
    std::cerr<<"NO MATERIAL FOUND in getMaterial!"<<std::endl;
    return Material();
  }else{
    return it->second;
  }
}
void MaterialManager::setMaterial(CollisionType first, CollisionType second, Material material) {
  CollisionPair pair{
    .first = first,
    .second = second
  };
  materialMap[pair] = material;
}
bool MaterialManager::contactAddedCallback(btManifoldPoint &cp,
                                           const btCollisionObjectWrapper *colObj0Wrap,
                                           int partId0,
                                           int index0,
                                           const btCollisionObjectWrapper *colObj1Wrap,
                                           int partId1,
                                           int index1) {

  CollisionType obj0Type = (CollisionType) colObj0Wrap->m_collisionObject->getBroadphaseHandle()->m_collisionFilterGroup;
  CollisionType obj1Type = (CollisionType) colObj1Wrap->m_collisionObject->getBroadphaseHandle()->m_collisionFilterGroup;
  if(obj0Type == CollisionType::COL_NOTHING || obj1Type == COL_NOTHING){
    return false;
  }
  CollisionPair pair{
    .first = obj0Type,
    .second = obj1Type
  };
  auto it = materialMap.find(pair);
  if(it == materialMap.end()){
    //std::cerr<<"Could not find materials in map! type_0: " << obj0Type<<" type_1: " <<obj1Type<<std::endl;
    return false;
  }
  const Material& material = it->second;
  return processMaterial(cp,material);
}
bool MaterialManager::processMaterial(btManifoldPoint &cp, const Material &material) {
  bool wasEdited = false;

  int flags = cp.m_contactPointFlags;
  flags = flags | material.addContactPointFlags;
  if(flags != cp.m_contactPointFlags){
    wasEdited = true;
    cp.m_contactPointFlags = flags;
  }
  if(material.friction.has_value()){
    wasEdited = true;
    cp.m_combinedFriction = material.friction.value();
  }

  if(material.rollingFriction.has_value()){
    wasEdited = true;
    cp.m_combinedRollingFriction= material.rollingFriction.value();
  }

  if(material.spinningFriction.has_value()){
    wasEdited = true;
    cp.m_combinedSpinningFriction = material.spinningFriction.value();
  }

  if(material.restitution.has_value()){
    wasEdited = true;
    cp.m_combinedRestitution = material.restitution.value();
  }

  if(material.frictionCFM.has_value()){
    wasEdited = true;
    cp.m_frictionCFM = material.frictionCFM.value();
  }

  if(material.contactERP.has_value()){
    wasEdited = true;
    cp.m_contactERP = material.contactERP.value();
  }

  if(material.contactCFM.has_value()){
    wasEdited = true;
    cp.m_contactCFM = material.contactCFM.value();
  }

  return wasEdited;
}
