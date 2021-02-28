//
// Created by rolf on 27-02-21.
//

#ifndef ROBOTEAM_MIMIR_VISUALIZERDEBUGDRAWER_H
#define ROBOTEAM_MIMIR_VISUALIZERDEBUGDRAWER_H

#include <btBulletDynamicsCommon.h>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

class VisualizerDebugDrawer : btIDebugDraw {
public:
    explicit VisualizerDebugDrawer(btDiscreteDynamicsWorld *world, double scale);
    ~VisualizerDebugDrawer() override;

    void init(QOpenGLShaderProgram * shader);
    void draw(QOpenGLShaderProgram * shader, QOpenGLFunctions * gl);

    // virtual functions we need to override from btIDebugDraw in order to implement the interface
    // most of these are empty functions since we don't actually need them but we need to define them.
    void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
    void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor) override;

    void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
    void drawSphere(btScalar radius, const btTransform& transform, const btVector3& color) override;
    void reportErrorWarning(const char* warningString) override;
    void draw3dText(const btVector3& location, const char* textString) override;
    void setDebugMode(int debugMode)override ;
    [[nodiscard]] int getDebugMode() const override;
private:
    void addLine(const btVector3& from, const btVector3& to, const btVector3& color);
    void addLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor);

    struct LineVertexData{
        float pos[3];
        float color[3];
    };

    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    std::vector<LineVertexData> linesData;

    int debugMode;
    double scale;
    btDiscreteDynamicsWorld *world;

    double contact_normal_length = 0.2;
    double sphere_basis_relative_length = 3.0;
};


#endif //ROBOTEAM_MIMIR_VISUALIZERDEBUGDRAWER_H
