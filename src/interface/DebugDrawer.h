//
// Created by rolf on 22-09-19.
//

#ifndef ROBOTEAM_MIMIR_DEBUGDRAWER_H
#define ROBOTEAM_MIMIR_DEBUGDRAWER_H

#include <bullet/btBulletDynamicsCommon.h>
namespace interface{
    class DebugVisualization;
    class DebugDrawer : public btIDebugDraw {

    public:
        explicit DebugDrawer(DebugVisualization* _visualization);
        ~DebugDrawer() override;

        // virtual functions we need to override from btIDebugDraw in order to implement the interface
        // most of these are empty functions since we don't actually need them but we need to define them.
        void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
        void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor) override;
        void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
        void drawSphere(btScalar radius, const btTransform& transform, const btVector3& color) override;
        void reportErrorWarning(const char* warningString) override;
        void draw3dText(const btVector3& location, const char* textString) override;
        void setDebugMode(int debugMode)override ;
        int getDebugMode() const override;
    private:
        DebugDrawModes debugMode;
        DebugVisualization * visualization;


    };
}



#endif //ROBOTEAM_MIMIR_DEBUGDRAWER_H
