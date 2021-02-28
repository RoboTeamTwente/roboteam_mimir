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
    vbo.allocate(&data.front(),data.size()*sizeof(VertexData));

    vao.release();
    vbo.release();
}

void VisualizerBox::draw(QOpenGLShaderProgram *shader, QOpenGLFunctions *gl) {
    vbo.bind();
    vao.bind();
    //we use the identity matrix,as the lines are already drawn in world coordinates
    QMatrix4x4 model;
    model.setToIdentity();
    shader->setUniformValue("model",model);

    gl->glDrawArrays(GL_TRIANGLES,0,data.size());
    vao.release();
    vbo.release();

}

void VisualizerBox::createVertexes(float length_x, float length_y, float length_z, float r, float g, float b) {
    data.reserve(36);
    float half_x = 0.5f*length_x;
    float half_y = 0.5f*length_y;
    float half_z = 0.5f*length_z;

//    addVertex(-half_x,-half_y,half_z,r,g,b);//0
//    addVertex(half_x,-half_y,half_z,r,g,b);//1
//    addVertex(half_x,half_y,half_z,r,g,b);//2
//    addVertex(-half_x,half_y,half_z,r,g,b);//3
//
//    addVertex(-half_x,-half_y,-half_z,r,g,b);//4
//    addVertex(half_x,-half_y,-half_z,r,g,b);//5
//    addVertex(half_x,half_y,-half_z,r,g,b);//6
//    addVertex(-half_x,half_y,-half_z,r,g,b);//7

    addVertex(-half_x,-half_y,half_z,r,g,b);//0
    addVertex(half_x,-half_y,half_z,r,g,b);//1
    addVertex(half_x,half_y,half_z,r,g,b);//2

    addVertex(half_x,half_y,half_z,r,g,b);//2
    addVertex(-half_x,half_y,half_z,r,g,b);//3
    addVertex(-half_x,-half_y,half_z,r,g,b);//0

    addVertex(half_x,-half_y,half_z,r,g,b);//1
    addVertex(half_x,-half_y,-half_z,r,g,b);//5
    addVertex(half_x,half_y,-half_z,r,g,b);//6

    addVertex(half_x,half_y,-half_z,r,g,b);//6
    addVertex(half_x,half_y,half_z,r,g,b);//2
    addVertex(half_x,-half_y,half_z,r,g,b);//1

    addVertex(-half_x,half_y,-half_z,r,g,b);//7
    addVertex(half_x,half_y,-half_z,r,g,b);//6
    addVertex(half_x,-half_y,-half_z,r,g,b);//5

    addVertex(half_x,-half_y,-half_z,r,g,b);//5
    addVertex(-half_x,-half_y,-half_z,r,g,b);//4
    addVertex(-half_x,half_y,-half_z,r,g,b);//7

    addVertex(-half_x,-half_y,-half_z,r,g,b);//4
    addVertex(-half_x,-half_y,half_z,r,g,b);//0
    addVertex(-half_x,half_y,half_z,r,g,b);//3

    addVertex(-half_x,half_y,half_z,r,g,b);//3
    addVertex(-half_x,half_y,-half_z,r,g,b);//7
    addVertex(-half_x,-half_y,-half_z,r,g,b);//4

    addVertex(-half_x,-half_y,-half_z,r,g,b);//4
    addVertex(half_x,-half_y,-half_z,r,g,b);//5
    addVertex(half_x,-half_y,half_z,r,g,b);//1

    addVertex(half_x,-half_y,half_z,r,g,b);//1
    addVertex(-half_x,-half_y,half_z,r,g,b);//0
    addVertex(-half_x,-half_y,-half_z,r,g,b);//4

    addVertex(-half_x,half_y,half_z,r,g,b);//3
    addVertex(half_x,half_y,half_z,r,g,b);//2
    addVertex(half_x,half_y,-half_z,r,g,b);//6

    addVertex(half_x,half_y,-half_z,r,g,b);//6
    addVertex(-half_x,half_y,-half_z,r,g,b);//7
    addVertex(-half_x,half_y,half_z,r,g,b);//3


}

void VisualizerBox::addVertex(float x, float y, float z, float r, float g, float b) {
    data.emplace_back(VertexData{.pos = {x,y,z},.color={r,g,b}});
}
