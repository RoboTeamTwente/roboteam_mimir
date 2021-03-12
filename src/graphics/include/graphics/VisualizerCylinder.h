//
// Created by rolf on 28-02-21.
//

#ifndef ROBOTEAM_MIMIR_VISUALIZERCYLINDER_H
#define ROBOTEAM_MIMIR_VISUALIZERCYLINDER_H


#include <vector>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class VisualizerCylinder {
public:
    //creates a cylinder around the x-axis
    VisualizerCylinder(float height, float radius, float r, float g, float b);
    void init(QOpenGLShaderProgram * shader);
    void draw(QOpenGLShaderProgram * shader, QOpenGLFunctions * gl,QMatrix4x4 model);
private:

    struct VertexData{
        float pos[3];
        float color[3];
        float normal[3];
    };

    std::vector<VertexData> data;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;

};


#endif //ROBOTEAM_MIMIR_VISUALIZERCYLINDER_H
