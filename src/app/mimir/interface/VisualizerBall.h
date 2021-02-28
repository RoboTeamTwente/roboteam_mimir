//
// Created by rolf on 27-02-21.
//

#ifndef ROBOTEAM_MIMIR_VISUALIZERBALL_H
#define ROBOTEAM_MIMIR_VISUALIZERBALL_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class VisualizerBall {
public:
    VisualizerBall(float radius, float r, float g, float b);
    void init(QOpenGLShaderProgram * shader);
    void draw(QOpenGLShaderProgram * shader, QOpenGLFunctions * gl);
private:
    void sphereEQ(float u, float v, float radius, float * write_to);
    void addTriangle(float a[3], float b[3], float c[3], float r, float g, float blue);
    struct VertexData {
    float pos[3];
    float color[3];
    };
    std::vector<VertexData> data;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
};


#endif //ROBOTEAM_MIMIR_VISUALIZERBALL_H
