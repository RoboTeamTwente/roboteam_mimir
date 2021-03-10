//
// Created by rolf on 20-02-21.
//

#include "Visualizer3D.h"
#include <QWheelEvent>
#include <iostream>
#include <QDir>
#include <QApplication>

void Visualizer3D::keyPressEvent(QKeyEvent *event) {
    if (event->key()==Qt::Key_W){
        upOn=true;
    }
    if (event->key()==Qt::Key_S){
        downOn=true;
    }
    if (event->key()==Qt::Key_A){
        leftOn=true;
    }
    if (event->key()==Qt::Key_D){
        rightOn=true;
    }
}

void Visualizer3D::wheelEvent(QWheelEvent *event) {
    const float zoomScale=20.0;
    float zoom_in_factor = (float)event->delta()/zoomScale;
    camera.zoom(zoom_in_factor);
}

void Visualizer3D::keyReleaseEvent(QKeyEvent *event) {
    if (event->key()==Qt::Key_W){
        upOn=false;
    }
    if (event->key()==Qt::Key_S){
        downOn=false;
    }
    if (event->key()==Qt::Key_A){
        leftOn=false;
    }
    if (event->key()==Qt::Key_D){
        rightOn=false;
    }
}

void Visualizer3D::mousePressEvent(QMouseEvent *event) {
    mouse_pressed = true;
    mouse_pressed_pos = event->pos();
}

void Visualizer3D::mouseReleaseEvent(QMouseEvent *event) {
    mouse_pressed = false;
}

void Visualizer3D::mouseMoveEvent(QMouseEvent *event) {
    if(!mouse_pressed){
        return;
    }
    QPoint delta=event->pos()-mouse_pressed_pos;
    if(first_press){
        delta = QPoint(0,0);
        first_press = false;
    }
    mouse_pressed_pos = event->pos();
    float angle_speed = 0.2;
    float yaw_change = (float) delta.x()*angle_speed;
    float pitch_change=  (float) delta.y()*angle_speed;
    camera.adjust_view_direction(yaw_change,pitch_change);
}

void Visualizer3D::resizeGL(int w, int h) {
    camera.window_resize(w,h);
}

Visualizer3D::Visualizer3D(double scale, QWidget *parent) : QOpenGLWidget(parent),
scale{scale}{
    setFocusPolicy(Qt::ClickFocus);
    setFocus();

  timer.start(20,this); //TODO: figure out better way to do constant updates.

}

void Visualizer3D::addDebugDrawing(btDiscreteDynamicsWorld *physics_world) {
    debugDrawer = new VisualizerDebugDrawer(physics_world,scale);
}

void Visualizer3D::initializeGL() {
    initializeOpenGLFunctions();

    debug_logger = new QOpenGLDebugLogger(nullptr);
    if(debug_logger->initialize()){
        std::cout<<"logging errors"<<std::endl;
        connect(debug_logger,SIGNAL(messageLogged(QOpenGLDebugMessage)),this,SLOT(messageLogged(QOpenGLDebugMessage)));
        debug_logger->startLogging();
    }


    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);//enable depth buffer
    glEnable(GL_CULL_FACE); //enable backface culling


    setupShader();

    shader->bind();
    debugDrawer->init(shader);
    visualWorld->init_gl(shader);
    shader->release();

}

void Visualizer3D::setupShader() {
    shader = new QOpenGLShaderProgram();
    //TODO: fix pathing shaders
    QString dir = findShaderDir();
    bool loadv = shader->addShaderFromSourceFile(QOpenGLShader::Vertex,dir+"/vertex_shader.glsl");
    bool loadf = shader->addShaderFromSourceFile(QOpenGLShader::Fragment,dir+"/fragment_shader.glsl");
    bool link = shader->link();
    if (!loadf || !loadv || !link) {
        std::cerr<<"Could not find, link or compile shaders!"<<std::endl;
        qDebug()<<shader->log();
        close();// close the widget leading to a white screen
    }
}

QString Visualizer3D::findShaderDir() {
    //ugly but we need to find a way to find the shaders
    QString exePath = QCoreApplication::applicationDirPath();
    QDir currentDir(exePath);
    bool success = true;
    while (currentDir.dirName() != "roboteam_mimir") {
        if (currentDir.isRoot()) {
            success = false;
            break;
        }
        currentDir.cdUp();
    }
    if (success) {
        if (currentDir.cd("src/app/mimir/interface")) {
            return currentDir.absolutePath();
        }
    }
    std::cerr << "Could not find the shader directory!" << std::endl;
    return currentDir.absolutePath();
}


void Visualizer3D::paintGL() {
    shader->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the buffers from previous iteration
    setupView();

  //every object needs to;
    //1 set the model transformation matrix
    //2. bind their own vbo's and vbo's
    //3. draw what they want to draw
    //4. unbind their buffers again
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    debugDrawer->draw(shader,f);
    visualWorld->draw(shader,f);

    shader->release();
}

void Visualizer3D::setupView() {
    shader->setUniformValue("projection",camera.getProjectionMatrix());
    shader->setUniformValue("view",camera.getViewMatrix());
    shader->setUniformValue("viewPos",camera.getViewPosition());

}

void Visualizer3D::timerEvent(QTimerEvent *event) {
    moveCamera();
    update();
}

void Visualizer3D::moveCamera() {
    if(upOn){
        camera.moveUp();
    }
    if(downOn){
        camera.moveDown();
    }
    if(leftOn){
        camera.moveLeft();
    }
    if(rightOn){
        camera.moveRight();
    }

}

void Visualizer3D::messageLogged(const QOpenGLDebugMessage& msg) {
    std::cerr<<"[OPENGL_ERROR]: "<<msg.message().toStdString()<<std::endl;
}
void Visualizer3D::addVisualWorld(VisualizedSimWorld *sim_world) {
  visualWorld = sim_world;
}
