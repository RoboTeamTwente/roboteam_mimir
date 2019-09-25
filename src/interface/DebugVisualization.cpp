//
// Created by rolf on 24-09-19.
//

#include "DebugVisualization.h"
#include <QOpenGLFunctions>
#include <QCoreApplication>
#include <QtCore/QDir>
#include "iostream"

namespace interface {

    DebugVisualization::DebugVisualization(QWidget *parent) : QOpenGLWidget(parent) {

    }
    DebugVisualization::~DebugVisualization() {
        // make sure OpenGL functions etc. are destroyed to prevent memory leaks (also in GPU)
        makeCurrent();

        delete shader;
        vbo.destroy();
        doneCurrent();
    }
    // This function initializes everything GL. Is only called once to start up everything to do with GL
    void DebugVisualization::initializeGL() {
        QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
        //initialize openGL buffers
        bool vboCreate = vbo.create();
        bool vaoCreate = vao.create();
        if (vao.isCreated()) {
            vao.bind();
        }
        //initialize shaders
        setupShaders();
        //create a black background
        f->glClearColor(0.0, 0.0, 0.0, 1.0);
        f->glEnable(GL_DEPTH_TEST);//enable depth buffer
        f->glEnable(GL_CULL_FACE); //enable backface culling
    }
    // Called whenever window size is changed to detail how visualization should change.
    void DebugVisualization::resizeGL(int w, int h) {

    }
    // Draws the scene
    void DebugVisualization::paintGL() {

        //clear buffers so that throw out old visualizations
        QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

        f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the buffers from previous iteration
        //set up the view
        QMatrix4x4 projection;
        projection.perspective(45.0, 4.0 / 3.0, 0.1, 100.0);
        QMatrix4x4 view;
        view.lookAt({4.0, 4.0, 3.0}, {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0});
        QMatrix4x4 model;
        model.setToIdentity();
        QMatrix4x4 mvp = projection * view * model;

        shaderProgram.setUniformValue("mvp_matrix", mvp);
        vbo.bind();
        //tell the shaders where in the buffers to look for the position and colors
        quintptr offset = 0;
        int vertexLocation = shaderProgram.attributeLocation("aPos");
        shaderProgram.enableAttributeArray(vertexLocation);
        shaderProgram.setAttributeBuffer(vertexLocation, GL_FLOAT,offset, 3,
                                         sizeof(VertexData));// each position has 3 floats.

        offset += sizeof(QVector3D);

        int colorLocation = shaderProgram.attributeLocation("color");
        shaderProgram.enableAttributeArray(colorLocation);
        shaderProgram.setAttributeBuffer(colorLocation, GL_FLOAT, offset, 4, sizeof(VertexData));

        QVector4D red(1.0,0.0,0.0,1.0);
        QVector4D blue(0.0,0.0,1.0,1.0);
        QVector4D green(0.0,1.0,0.0,1.0);
        QVector4D yellow(1.0,1.0,0.0,1.0);
        lines.push_back({QVector3D(-0.5f, -0.5f, 0.0f),red});
        lines.push_back({QVector3D(0.5f, -0.5f, 0.0f), blue });
        lines.push_back({QVector3D(0.5f, -0.5f, 0.0f), blue});
        lines.push_back({QVector3D(0.0f, 0.5f, 0.0f), green});
        lines.push_back({QVector3D(0.0f, 0.5f, 0.0f), green});
        lines.push_back({QVector3D(-0.5f, -0.5f, 0.0f), red});
        lines.push_back({QVector3D(0.0f, 0.0f, -0.5f), yellow});
        lines.push_back({QVector3D(-0.5f, -0.5f, 0.0f), red});
        lines.push_back({QVector3D(0.0f, 0.0f, -0.5f), yellow});
        lines.push_back({QVector3D(0.5f, -0.5f, 0.0f), blue});
        lines.push_back({QVector3D(0.0f, 0.0f, -0.5f), yellow});
        lines.push_back({QVector3D(0.0f, 0.5f, 0.0f), green});

        VertexData vertices[lines.size()];
        std::copy(lines.begin(), lines.end(), vertices);
        vbo.allocate(vertices, lines.size() * sizeof(VertexData));


        f->glDrawArrays(GL_LINES, 0, lines.size());
        lines.clear();
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

}