//
// Created by rolf on 24-11-19.
//

#ifndef ROBOTEAM_MIMIR_COLLISIONSHARED_H
#define ROBOTEAM_MIMIR_COLLISIONSHARED_H
#include <btBulletDynamicsCommon.h>
#include <optional>
#include <unordered_map>

enum CollisionType{
    COL_NOTHING=0,
    COL_GROUND = 1,
    COL_BALL=2,
    COL_ROBOT_HULL=4,
    COL_ROBOT_WHEEL = 8,
    COL_ROBOT_DRIBBLER = 16,
    COL_WALL = 32,
    COL_CAMERA_RAY=64,
    COL_EVERYTHING = 127
};
struct CollisionPair{
  CollisionType first;
  CollisionType second;
  bool operator==(const CollisionPair& other) const{
    return (first == other.first  && second == other.second) || (first == other.second && second == other.first);
  }
  bool operator!=(const CollisionPair& other) const{
    return !(*this == other);
  }

};
class CollisionTypeHasher {
 public:
  size_t operator () (const CollisionPair& pair) const{
    CollisionType smallest = pair.first < pair.second ? pair.first : pair.second;
    CollisionType largest = pair.first < pair.second ? pair.second : pair.first;
    size_t h = (size_t(smallest)<<32)+size_t(largest);
    h*=1231231557ull; // "random" uneven integer
    h^=(h>>32);
    return h;
  }
};
//Describes the collision properties of two materials (a collision 'Manifold')
struct Material{
  int addContactPointFlags = 0; //flags which are *added* to the existing flags of an object
  std::optional<btScalar> friction = std::nullopt;
  std::optional<btScalar> rollingFriction = std::nullopt;
  std::optional<btScalar> spinningFriction = std::nullopt;
  std::optional<btScalar> restitution = std::nullopt;

  std::optional<btScalar> frictionCFM = std::nullopt;
  std::optional<btScalar> contactERP = std::nullopt;
  std::optional<btScalar> contactCFM = std::nullopt;

};

typedef bool (*ContactAddedCallback)(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1);
bool materialCallback(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1);

//usually not a fan of singletons, but in this case we actually need a single instance, because the callback function from
//bullet is a global variable; so there are not a lot of other save options here, as we need a static callback
class MaterialManager{
 public:
  static void initialize();
  static void deinitialize();
  static Material getMaterial(CollisionType first,CollisionType second);
  static void setMaterial(CollisionType first, CollisionType second, Material material);
 private:
  static bool contactAddedCallback(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1);
  static bool processMaterial(btManifoldPoint& cp, const Material& material);
  static std::unordered_map<CollisionPair,Material,CollisionTypeHasher> materialMap;
};

#endif //ROBOTEAM_MIMIR_COLLISIONSHARED_H
