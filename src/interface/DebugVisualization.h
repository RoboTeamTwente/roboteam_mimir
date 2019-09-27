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
#include <QBasicTimer>
namespace interface {
    //helper struct
    struct VertexData {
        btVector3 pos;
        btVector3 color;
    };

    class DebugDrawer;

    class DebugVisualization : public QOpenGLWidget, QOpenGLFunctions {
    Q_OBJECT
    public:
        explicit DebugVisualization(btDiscreteDynamicsWorld *world, QWidget *parent = nullptr);
        ~DebugVisualization();
        void addLine(const btVector3 &from, const btVector3 &to, const btVector3 &color);
        void addLine(const btVector3 &from, const btVector3 &to, const btVector3 &fromColor,const btVector3 &toColor);
    protected:
        void initializeGL() override;
        void resizeGL(int w, int h) override;
        void paintGL() override;
        void timerEvent(QTimerEvent *event) override;
        void keyPressEvent(QKeyEvent *event) override;
    private:
        void draw();
        void setupShaders();
        void setupView();
        QString findShaderDir();

        float viewAngle=45.0;
        float aspect=4.0/3.0;
        QMatrix4x4 projection;
        QVector3D cameraPos={0.0,2.0,4.0};
        QVector3D cameraFront=QVector3D(-cameraPos).normalized();
        QVector3D cameraUp={0.0,1.0,0.0};

        QOpenGLVertexArrayObject vao;
        QOpenGLBuffer lineVbo;
        QOpenGLShader *shader;
        QOpenGLShaderProgram shaderProgram;
        std::vector<VertexData> lines;
        DebugDrawer *drawer;
        btDiscreteDynamicsWorld *world;

        QBasicTimer timer;
    };
}


#endif //ROBOTEAM_MIMIR_DEBUGVISUALIZATION_H
