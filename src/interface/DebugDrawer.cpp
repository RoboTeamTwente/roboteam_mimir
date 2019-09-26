//
// Created by rolf on 22-09-19.
//

#include "DebugDrawer.h"
#include <iostream>
#include "DebugVisualization.h"
namespace interface {

    DebugDrawer::DebugDrawer(DebugVisualization *_visualization) {
        visualization=_visualization;
    }
    DebugDrawer::~DebugDrawer() {

    }
    void DebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) {
        visualization->addLine(from,to,color);// magic!
    }
    void DebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &fromColor, const btVector3 &toColor) {
        visualization->addLine(from,to,fromColor,toColor);
    }
    void DebugDrawer::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance,
                                       int lifeTime, const btVector3 &color) {

    }
    //sets the debug mode; what is drawn by the drawer
    int DebugDrawer::getDebugMode() const {
        return DBG_DrawWireframe | DBG_DrawAabb;
    }
    void DebugDrawer::setDebugMode(int _debugMode) {
        debugMode = static_cast<DebugDrawModes>(_debugMode);
    }
    void DebugDrawer::reportErrorWarning(const char *warningString) {
        std::cerr << "[Debug drawer]: " + std::string(warningString) << std::endl;
    }

    void DebugDrawer::draw3dText(const btVector3 &location, const char *textString) {

    }
}