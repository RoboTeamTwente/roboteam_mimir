//
// Created by rolf on 24-09-19.
//

#include "DebugVisualization.h"
#include "QOpenGLFunctions"

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
    // This function initializes everything GL. Is only called once to startUp GL contexts
    void DebugVisualization::initializeGL() {
        QOpenGLFunctions *f=QOpenGLContext::currentContext()->functions();

        //initialize openGL resources
        bool vboCreate=vbo.create();
        bool vaoCreate=vao.create();
        if (vao.isCreated()){
            vao.bind();
        }
        //initialize shaders
        bool loadVShader=shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,"/home/rolf/roboteamtwente/roboteam_mimir/src/interface/vshader.glsl");
        bool loadFShader=shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,"/home/rolf/roboteamtwente/roboteam_mimir/src/interface/fshader.glsl");
        bool link= shaderProgram.link();
        bool bind= shaderProgram.bind();
        if (!loadFShader||!loadVShader||!link||!bind){
            close();// close the widget leading to a white screen
        }
        //create a black background
        f->glClearColor(0.0,0.0,0.0,1.0);
        float vertices[] = {
                -0.5f, -0.5f, 0.0f,1.0f,
                0.5f, -0.5f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.0f, 1.0f,
                0.0f,  0.5f, 0.0f, 1.0f,
                0.0f,  0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.0f,1.0f,
                0.0f, 0.0f, -0.5f, 1.0f,
                -0.5f, -0.5f, 0.0f,1.0f,
                0.0f, 0.0f, -0.5f, 1.0f,
                0.5f, -0.5f, 0.0f, 1.0f,
                0.0f, 0.0f, -0.5f, 1.0f,
                0.0f,  0.5f, 0.0f, 1.0f
        };
        f->glEnable(GL_DEPTH_TEST);//enable depth buffer
        f->glEnable(GL_CULL_FACE); //enable backface culling
        vbo.bind();
        vbo.allocate(vertices,12*4*sizeof(float));
    }
    // Called whenever window size is changed to detail how visualization should change.
    void DebugVisualization::resizeGL(int w, int h) {

    }
    // Draws the scene
    void DebugVisualization::paintGL() {
        //clear buffers so that throw out old visualizations
        QOpenGLFunctions *f=QOpenGLContext::currentContext()->functions();
        f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the buffers from previous iteration
        //set up the view
        QMatrix4x4 projection;
        projection.perspective(45.0,4.0/3.0,0.1,100.0);
        QMatrix4x4 view;

        view.lookAt({4.0,4.0,3.0},{0.0,0.0,0.0},{0.0,1.0,0.0});
        QMatrix4x4 model;
        model.setToIdentity();
        QMatrix4x4 mvp=projection*view*model;
        shaderProgram.setUniformValue("mvp_matrix",mvp);
        vbo.bind();
        quintptr offset = 0;
        int vertexLocation= shaderProgram.attributeLocation("aPos");
        shaderProgram.enableAttributeArray(vertexLocation);
        shaderProgram.setAttributeBuffer(vertexLocation,GL_FLOAT, offset, 4);
        f->glDrawArrays(GL_LINES, 0, 12);


    }

}