//
// Created by rolf on 20-02-21.
//

#ifndef ROBOTEAM_MIMIR_VISUALIZER3D_H
#define ROBOTEAM_MIMIR_VISUALIZER3D_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include "VisualizerCamera.h"
#include "VisualizerDebugDrawer.h"
#include "VisualizerWorld.h"
#include <QOpenGLDebugLogger>
#include <QOpenGLDebugMessage>

class Visualizer3D : public QOpenGLWidget, QOpenGLFunctions{
Q_OBJECT
public:
    explicit Visualizer3D(double scale, QWidget * parent = nullptr);
    void addDebugDrawing(btDiscreteDynamicsWorld * physics_world);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void timerEvent(QTimerEvent *event) override;

protected slots:
    void messageLogged(const QOpenGLDebugMessage &msg);
private:
    QString findShaderDir();
    void setupShader();
    void setupDebugDrawer();
    void setupView();
    void moveCamera();

    bool upOn = false;
    bool downOn = false;
    bool leftOn = false;
    bool rightOn = false;
    //controls
    bool first_press = true;
    bool mouse_pressed = false;
    QPoint mouse_pressed_pos;

    VisualizerCamera camera;
    double scale;

    QOpenGLShaderProgram * shader;

    VisualizerDebugDrawer * debugDrawer;
    VisualizerWorld * world;

    QBasicTimer timer;
    QOpenGLDebugLogger * debug_logger;
};


#endif //ROBOTEAM_MIMIR_VISUALIZER3D_H
