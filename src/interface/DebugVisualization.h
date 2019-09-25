//
// Created by rolf on 24-09-19.
//

#ifndef ROBOTEAM_MIMIR_DEBUGVISUALIZATION_H
#define ROBOTEAM_MIMIR_DEBUGVISUALIZATION_H

#include <QOpenGLWidget>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLShader>
#include <QtGui/QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <btBulletDynamicsCommon.h>

namespace interface {
    //helper struct
    struct VertexData {
        btVector3 pos;
        btVector3 color;
    };

    class DebugDrawer;

    class DebugVisualization : public QOpenGLWidget {
    Q_OBJECT
    public:
        explicit DebugVisualization(btDiscreteDynamicsWorld *world, QWidget *parent = nullptr);
        ~DebugVisualization();
        void addLine(const btVector3 &p1, const btVector3 &p2, const btVector3 &color);
    protected:
        void initializeGL() override;
        void resizeGL(int w, int h) override;
        void paintGL() override;
    private:
        void draw(QOpenGLFunctions *f);
        void setupShaders();
        QString findShaderDir();

        QOpenGLVertexArrayObject vao;
        QOpenGLBuffer vbo;
        QOpenGLShader *shader;
        QOpenGLShaderProgram shaderProgram;
        std::vector<VertexData> lines;
        DebugDrawer *drawer;
        btDiscreteDynamicsWorld *world;
    };
}


#endif //ROBOTEAM_MIMIR_DEBUGVISUALIZATION_H
