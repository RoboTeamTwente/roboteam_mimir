//
// Created by rolf on 27-02-21.
//

#ifndef ROBOTEAM_MIMIR_VISUALIZERBOX_H
#define ROBOTEAM_MIMIR_VISUALIZERBOX_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class VisualizerBox {
public:
    // local coordinates of the box are around (0,0), with half extents in each dimension
    VisualizerBox(float half_x, float half_y, float half_z,
                  float r, float g, float b); //TODO: make a color class
    void init(QOpenGLShaderProgram * shader);
    void draw(QOpenGLShaderProgram * shader, QOpenGLFunctions * gl,const QMatrix4x4& mat);
private:
    void createVertexes(float length_x, float length_y, float length_z, float r, float g, float b);
    void addVertex(float x, float y, float z, float r, float g, float b);
    struct VertexData{
        float pos[3];
        float color[3];
    };
    std::vector<VertexData> data;
    QOpenGLVertexArrayObject vao;

    QOpenGLBuffer vbo;
};


#endif //ROBOTEAM_MIMIR_VISUALIZERBOX_H
