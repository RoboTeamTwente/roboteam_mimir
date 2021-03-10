//
// Created by rolf on 27-02-21.
//

#include "VisualizerDebugDrawer.h"
#include <iostream>

int VisualizerDebugDrawer::getDebugMode() const {
    return debugMode;
}



void VisualizerDebugDrawer::addLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) {
    LineVertexData first{
        .pos = {from.x(),from.y(),from.z()},
        .color = {color.x(),color.y(),color.z()}
    };
    linesData.push_back(first);
    LineVertexData second{
            .pos = {to.x(),to.y(),to.z()},
            .color = {color.x(),color.y(),color.z()}
    };
    linesData.push_back(second);

}
void VisualizerDebugDrawer::addLine(const btVector3 &from, const btVector3 &to, const btVector3 &from_color, const btVector3& to_color) {
    LineVertexData first{
            .pos = {from.x(),from.y(),from.z()},
            .color = {from_color.x(),from_color.y(),from_color.z()}
    };
    linesData.push_back(first);
    LineVertexData second{
            .pos = {to.x(),to.y(),to.z()},
            .color = {to_color.x(),to_color.y(),to_color.z()}
    };
    linesData.push_back(second);

}

void VisualizerDebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) {
    addLine(from/scale,to/scale,color);
}

void VisualizerDebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &fromColor,
                                     const btVector3 &toColor) {
    addLine(from/scale,to/scale,fromColor,toColor);
}

VisualizerDebugDrawer::VisualizerDebugDrawer(btDiscreteDynamicsWorld *world, double scale) : scale{scale}, world{world}{
    debugMode = DBG_DrawWireframe | DBG_DrawContactPoints |DBG_DrawConstraints;
    this->world->setDebugDrawer(this);

}
VisualizerDebugDrawer::~VisualizerDebugDrawer() {
    world->setDebugDrawer(nullptr); //could be world is already destructed here... maybe ensure this doesn't happen?

}

void VisualizerDebugDrawer::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance,
                                             int lifeTime, const btVector3 &color) {
    drawLine(PointOnB,PointOnB+normalOnB*contact_normal_length*scale,color);

}

void VisualizerDebugDrawer::reportErrorWarning(const char *warningString) {
    std::cerr << "[Debug drawer]: " + std::string(warningString) << std::endl;

}

void VisualizerDebugDrawer::draw3dText(const btVector3 &location, const char *textString) {
    //ignored, but must override
}

void VisualizerDebugDrawer::setDebugMode(int mode) {
    debugMode = static_cast<DebugDrawModes>(mode);
}

void VisualizerDebugDrawer::drawSphere(btScalar radius, const btTransform &transform, const btVector3 &color) {
    btIDebugDraw::drawSphere(radius,transform,color);
    drawLine(transform.getOrigin(),transform.getOrigin()+transform.getBasis().getColumn(1)*radius*sphere_basis_relative_length,color);
    drawLine(transform.getOrigin(),transform.getOrigin()+transform.getBasis().getColumn(0)*radius*sphere_basis_relative_length,color);
    drawLine(transform.getOrigin(),transform.getOrigin()+transform.getBasis().getColumn(2)*radius*sphere_basis_relative_length,color);
}

void VisualizerDebugDrawer::init(QOpenGLShaderProgram* shader) {

    vbo.create();
    vbo.bind();

    vao.create();
    vao.bind();
    quintptr offset = 0;

    int pos_location = shader->attributeLocation("pos");
    shader->enableAttributeArray(pos_location);
    shader->setAttributeBuffer(pos_location,GL_FLOAT,offset,3,sizeof(LineVertexData));

    offset += sizeof(float)*3;
    int color_location = shader->attributeLocation("color");
    shader->enableAttributeArray(color_location);
    shader->setAttributeBuffer(color_location,GL_FLOAT,offset,3,sizeof(LineVertexData));
    vao.release();
    vbo.release();
}

void VisualizerDebugDrawer::draw(QOpenGLShaderProgram * shader, QOpenGLFunctions * gl) {

    linesData.clear();
    if(world!=nullptr){
        world->debugDrawWorld();
    }
    vbo.bind();
    vao.bind();

    //we use the identity matrix,as the lines are already drawn in world coordinates
    QMatrix4x4 model;
    model.setToIdentity();
    shader->setUniformValue("model",model);
    shader->setUniformValue("compute_lighting",false);

    vbo.allocate(&linesData.front(), linesData.size() * sizeof(LineVertexData));
    gl->glDrawArrays(GL_LINES, 0, linesData.size());
    vao.release();
    vbo.release();
}
