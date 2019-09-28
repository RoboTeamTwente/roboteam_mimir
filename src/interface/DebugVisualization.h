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
        void keyReleaseEvent(QKeyEvent *event) override;
        void wheelEvent(QWheelEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
    private:
        void draw();
        void setupShaders();
        void setupView();
        QString findShaderDir();
        void moveCamera();

        float viewAngle=45.0;
        float aspect=4.0/3.0;
        QMatrix4x4 projection;
        float yaw=0.0;
        float pitch=0.0;
        QVector3D cameraPos={-3.0,0.0,0.0};
        QVector3D cameraFront=QVector3D(1.0,0.0,0.0);
        QVector3D cameraUp={0.0,0.0,1.0};
        bool upOn= false;
        bool leftOn= false;
        bool rightOn= false;
        bool downOn= false;
        bool mousePressed=false;
        QPoint mousePos;
        bool firstPress=true;


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
