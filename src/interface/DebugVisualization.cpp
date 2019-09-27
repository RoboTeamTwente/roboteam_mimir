//
// Created by rolf on 24-09-19.
//

#include "DebugVisualization.h"
#include <QOpenGLFunctions>
#include <QCoreApplication>
#include <QtCore/QDir>
#include "iostream"
#include "DebugDrawer.h"
#include <QKeyEvent>
namespace interface {

    DebugVisualization::DebugVisualization(btDiscreteDynamicsWorld *_world, QWidget *parent) : QOpenGLWidget(parent) {
        //connect debug drawer to the world.
        drawer=new DebugDrawer(this);
        world=_world;
        world->setDebugDrawer(drawer);
        setFocusPolicy(Qt::ClickFocus);
        setFocus();
    }
    DebugVisualization::~DebugVisualization() {
        delete drawer;
        // make sure OpenGL functions etc. are destroyed to prevent memory leaks (also in GPU)
        makeCurrent();

        delete shader;
        lineVbo.destroy();
        doneCurrent();
    }
    // This function initializes everything GL. Is only called once to start up everything to do with GL
    void DebugVisualization::initializeGL() {
        initializeOpenGLFunctions();
        //initialize openGL buffers
        lineVbo.create();
        vao.create();
        if (vao.isCreated()) {
            vao.bind();
        }
        lineVbo.bind();
        //initialize shaders
        setupShaders();
        //create a black background
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glEnable(GL_DEPTH_TEST);//enable depth buffer
        glEnable(GL_CULL_FACE); //enable backface culling
        setupView(); //setup camera view matrices

        timer.start(10,this);
    }
    void DebugVisualization::timerEvent(QTimerEvent *event) {
        moveCamera();// we do this with the timer only to keep cam speed constant
        update();//request an update
    }

    void DebugVisualization::moveCamera() {
        float speed=0.1;
        if (upOn){
            cameraPos+=cameraFront*speed;
        }
        if (downOn){
            cameraPos-=cameraFront*speed;
        }
        if (leftOn){
            cameraPos-=QVector3D::crossProduct(cameraFront,cameraUp).normalized()*speed;
        }
        if (rightOn){
            cameraPos+=QVector3D::crossProduct(cameraFront,cameraUp).normalized()*speed;
        }
    }
    void DebugVisualization::setupView()  {//set up the view
        QMatrix4x4 view;
        view.lookAt(cameraPos,cameraPos+cameraFront,cameraUp);
        shaderProgram.setUniformValue("mvp_matrix", projection * view ); //sets the projection matrix (from world to screen coordinates)

    }
    // Called whenever window size is changed to detail how visualization should change.
    void DebugVisualization::resizeGL(int w, int h) {
        aspect=(float) w/(float) h;
        projection.setToIdentity();
        projection.perspective(viewAngle, aspect, 0.1, 100.0);

    }
    void DebugVisualization::keyPressEvent(QKeyEvent *event) {

        std::cout<<"Key pressed"<<std::endl;
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
    void DebugVisualization::keyReleaseEvent(QKeyEvent *event) {
        std::cout<<"Key released"<<std::endl;
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
    // Draws the scene
    void DebugVisualization::paintGL() {
        //clear buffers so that throw out old visualizations
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the buffers from previous iteration
        setupView();
        world->debugDrawWorld();//this calls the drawer internally
        draw();
    }
    void DebugVisualization::setupShaders() {
        QString shaderDir = findShaderDir();
        bool loadVShader = shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, shaderDir + "/vshader.glsl");
        bool loadFShader = shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, shaderDir + "/fshader.glsl");
        bool bind = shaderProgram.bind();
        bool link = shaderProgram.link();

        if (!loadFShader || !loadVShader || !link || !bind) {
            close();// close the widget leading to a white screen
        }
    }
    QString DebugVisualization::findShaderDir() {
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
            if (currentDir.cd("src/interface")) {
                return currentDir.absolutePath();
            }
        }
        std::cerr << "Could not find the shader directory!" << std::endl;
        return currentDir.absolutePath();
    }
    void DebugVisualization::addLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) {
        lines.push_back({from,color});
        lines.push_back({to,color});
    }
    void DebugVisualization::addLine(const btVector3 &from, const btVector3 &to, const btVector3 &fromColor, const btVector3&toColor) {
        lines.push_back({from,fromColor});
        lines.push_back({to,toColor});
    }
    void DebugVisualization::draw(){
        //tell the shaders where in the buffers to look for the position and colors
        quintptr offset = 0;
        int vertexLocation = shaderProgram.attributeLocation("aPos");
        shaderProgram.enableAttributeArray(vertexLocation);
        shaderProgram.setAttributeBuffer(vertexLocation, GL_FLOAT,offset, 3,
                                         sizeof(VertexData));// each position has 3 floats.

        offset += sizeof(btVector3);
        int colorLocation = shaderProgram.attributeLocation("color");
        shaderProgram.enableAttributeArray(colorLocation);
        shaderProgram.setAttributeBuffer(colorLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

        //draw all the points and clear them again
        lineVbo.allocate(&lines.front(), lines.size() * sizeof(VertexData));
        glDrawArrays(GL_LINES, 0, lines.size());
        lines.clear();
    }

}