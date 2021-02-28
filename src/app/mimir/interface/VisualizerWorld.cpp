//
// Created by rolf on 27-02-21.
//

#include "VisualizerWorld.h"

VisualizerWorld::VisualizerWorld() {
    //box = new VisualizerBox(1.0,2.0,3.0,1.0,0.0,1.0);
    //ball = new VisualizerBall(1.5,0.0,1.0,0.0);
    cylinder = new VisualizerCylinder(0.5,0.15,1.0,0.0,0.0);
}

void VisualizerWorld::init(QOpenGLShaderProgram *shader) {
//    box->init(shader);
//    ball->init(shader);
    cylinder->init(shader);
}

void VisualizerWorld::draw(QOpenGLShaderProgram *shader, QOpenGLFunctions *gl) {
//    box->draw(shader,gl);
//    ball->draw(shader,gl);
    cylinder->draw(shader,gl);
}
