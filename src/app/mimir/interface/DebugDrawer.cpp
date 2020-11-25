//
// Created by rolf on 22-09-19.
//

#include "DebugDrawer.h"
#include <iostream>
#include "DebugVisualization.h"
namespace interface {
    DebugDrawer::DebugDrawer(DebugVisualization *_visualization,double scale) : scale(scale) {
        visualization=_visualization;
    }
    DebugDrawer::~DebugDrawer() {

    }
    void DebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) {
        visualization->addLine(from/scale,to/scale,color);// magic!
    }
    void DebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &fromColor, const btVector3 &toColor) {
        visualization->addLine(from/scale,to/scale,fromColor,toColor);
    }
    void DebugDrawer::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance,
                                       int lifeTime, const btVector3 &color) {
        drawLine(PointOnB,PointOnB+normalOnB*0.2*scale,color);
    }
    //sets the debug mode; what is drawn by the drawer
    int DebugDrawer::getDebugMode() const {
        return DBG_DrawWireframe | DBG_DrawContactPoints |DBG_DrawConstraints ;
    }
    void DebugDrawer::setDebugMode(int _debugMode) {
        debugMode = static_cast<DebugDrawModes>(_debugMode);
    }
    void DebugDrawer::reportErrorWarning(const char *warningString) {
        std::cerr << "[Debug drawer]: " + std::string(warningString) << std::endl;
    }
    void DebugDrawer::drawSphere(btScalar radius, const btTransform &transform, const btVector3 &color) {
        btIDebugDraw::drawSphere(radius,transform,color);
        drawLine(transform.getOrigin(),transform.getOrigin()+transform.getBasis().getColumn(1)*radius*3.0,color);
        drawLine(transform.getOrigin(),transform.getOrigin()+transform.getBasis().getColumn(0)*radius*3.0,color);
        drawLine(transform.getOrigin(),transform.getOrigin()+transform.getBasis().getColumn(2)*radius*3.0,color);
    }
    void DebugDrawer::draw3dText(const btVector3 &location, const char *textString) {
      std::cout<<"DD: "<<textString<<std::endl;
    }
}