//
// Created by rolf on 22-09-19.
//

#ifndef ROBOTEAM_MIMIR_DEBUGDRAWER_H
#define ROBOTEAM_MIMIR_DEBUGDRAWER_H

#include <bullet/btBulletDynamicsCommon.h>

class DebugDrawer : public btIDebugDraw {
    int m_debugMode;
public:
    DebugDrawer();
    virtual ~DebugDrawer();

    // virtual functions we need to override from btIDebugDraw in order to implement the interface
    // most of these are empty functions since we don't actually need them but we need to define them.
    virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor);
    virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
    virtual void drawSphere(const btVector3& p, btScalar radius, const btVector3& color);
    virtual void drawTriangle(const btVector3& a, const btVector3& b, const btVector3& c, const btVector3& color, btScalar alpha);
    virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
    virtual void reportErrorWarning(const char* warningString);
    virtual void draw3dText(const btVector3& location, const char* textString);
    virtual void setDebugMode(int debugMode);
    virtual int getDebugMode() const;
private:
    DebugDrawModes debugMode;

};


#endif //ROBOTEAM_MIMIR_DEBUGDRAWER_H
