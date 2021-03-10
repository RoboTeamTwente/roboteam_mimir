//
// Created by rolf on 27-02-21.
//

#include "VisualizerBox.h"

VisualizerBox::VisualizerBox(float length_x, float length_y, float length_z,
                             float r, float g, float b) {
    createVertexes(length_x,length_y,length_z,r,g,b);
}

void VisualizerBox::init(QOpenGLShaderProgram *shader) {
    vbo.create();
    vbo.bind();

    vao.create();
    vao.bind();
    quintptr offset = 0;

    int pos_location = shader->attributeLocation("pos");
    shader->enableAttributeArray(pos_location);
    shader->setAttributeBuffer(pos_location,GL_FLOAT,offset,3,sizeof(VertexData));

    offset += sizeof(float)*3;
    int color_location = shader->attributeLocation("color");
    shader->enableAttributeArray(color_location);
    shader->setAttributeBuffer(color_location,GL_FLOAT,offset,3,sizeof(VertexData));

    offset += sizeof(float)*3;
    int normal_location = shader->attributeLocation("objectNormal");
    shader->enableAttributeArray(normal_location);
    shader->setAttributeBuffer(normal_location,GL_FLOAT,offset,3,sizeof(VertexData));

    vbo.allocate(&data.front(),data.size()*sizeof(VertexData));

    vao.release();
    vbo.release();
}

void VisualizerBox::draw(QOpenGLShaderProgram *shader, QOpenGLFunctions *gl, const QMatrix4x4& model) {
    vbo.bind();
    vao.bind();

    shader->setUniformValue("model",model);
    gl->glDrawArrays(GL_TRIANGLES,0,data.size());

    vao.release();
    vbo.release();

}

void VisualizerBox::createVertexes(float length_x, float length_y, float length_z, float r, float g, float b) {
    data.reserve(36);
    float half_x = length_x;
    float half_y = length_y;
    float half_z = length_z;

//    addVertex(-half_x,-half_y,half_z,r,g,b);//0
//    addVertex(half_x,-half_y,half_z,r,g,b);//1
//    addVertex(half_x,half_y,half_z,r,g,b);//2
//    addVertex(-half_x,half_y,half_z,r,g,b);//3
//
//    addVertex(-half_x,-half_y,-half_z,r,g,b);//4
//    addVertex(half_x,-half_y,-half_z,r,g,b);//5
//    addVertex(half_x,half_y,-half_z,r,g,b);//6
//    addVertex(-half_x,half_y,-half_z,r,g,b);//7

    addVertex(-half_x,-half_y,half_z,r,g,b,0,0,1);//0
    addVertex(half_x,-half_y,half_z,r,g,b,0,0,1);//1
    addVertex(half_x,half_y,half_z,r,g,b,0,0,1);//2

    addVertex(half_x,half_y,half_z,r,g,b,0,0,1);//2
    addVertex(-half_x,half_y,half_z,r,g,b,0,0,1);//3
    addVertex(-half_x,-half_y,half_z,r,g,b,0,0,1);//0

    addVertex(half_x,-half_y,half_z,r,g,b,1,0,0);//1
    addVertex(half_x,-half_y,-half_z,r,g,b,1,0,0);//5
    addVertex(half_x,half_y,-half_z,r,g,b,1,0,0);//6

    addVertex(half_x,half_y,-half_z,r,g,b,1,0,0);//6
    addVertex(half_x,half_y,half_z,r,g,b,1,0,0);//2
    addVertex(half_x,-half_y,half_z,r,g,b,1,0,0);//1

    addVertex(-half_x,half_y,-half_z,r,g,b,0,0,-1);//7
    addVertex(half_x,half_y,-half_z,r,g,b,0,0,-1);//6
    addVertex(half_x,-half_y,-half_z,r,g,b,0,0,-1);//5

    addVertex(half_x,-half_y,-half_z,r,g,b,0,0,-1);//5
    addVertex(-half_x,-half_y,-half_z,r,g,b,0,0,-1);//4
    addVertex(-half_x,half_y,-half_z,r,g,b,0,0,-1);//7

    addVertex(-half_x,-half_y,-half_z,r,g,b,-1,0,0);//4
    addVertex(-half_x,-half_y,half_z,r,g,b,-1,0,0);//0
    addVertex(-half_x,half_y,half_z,r,g,b,-1,0,0);//3

    addVertex(-half_x,half_y,half_z,r,g,b,-1,0,0);//3
    addVertex(-half_x,half_y,-half_z,r,g,b,-1,0,0);//7
    addVertex(-half_x,-half_y,-half_z,r,g,b,-1,0,0);//4

    addVertex(-half_x,-half_y,-half_z,r,g,b,0,-1,0);//4
    addVertex(half_x,-half_y,-half_z,r,g,b,0,-1,0);//5
    addVertex(half_x,-half_y,half_z,r,g,b,0,-1,0);//1

    addVertex(half_x,-half_y,half_z,r,g,b,0,-1,0);//1
    addVertex(-half_x,-half_y,half_z,r,g,b,0,-1,0);//0
    addVertex(-half_x,-half_y,-half_z,r,g,b,0,-1,0);//4

    addVertex(-half_x,half_y,half_z,r,g,b,0,1,0);//3
    addVertex(half_x,half_y,half_z,r,g,b,0,1,0);//2
    addVertex(half_x,half_y,-half_z,r,g,b,0,1,0);//6

    addVertex(half_x,half_y,-half_z,r,g,b,0,1,0);//6
    addVertex(-half_x,half_y,-half_z,r,g,b,0,1,0);//7
    addVertex(-half_x,half_y,half_z,r,g,b,0,1,0);//3
}

void VisualizerBox::addVertex(float x, float y, float z, float r, float g, float b, float n_x, float n_y, float n_z) {
    data.emplace_back(VertexData{.pos = {x,y,z},.color={r,g,b},.normal{n_x,n_y,n_z}});
}
