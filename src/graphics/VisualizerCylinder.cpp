//
// Created by rolf on 28-02-21.
//

#include "VisualizerCylinder.h"
#include <cmath>

VisualizerCylinder::VisualizerCylinder(float height, float radius, float r, float g, float b) {
    //
    int circle_resolution = 10;
    float half_height = 0.5f*height;

    VertexData top_center{.pos ={half_height,0.0f,0.0f},.color ={r,g,b},.normal{0,0,1}};
    VertexData bottom_center{.pos ={-half_height,0.0f,0.0f},.color ={r,g,b},.normal{0,0,-1}};

    //top face
    float startAngle = 0.0;
    float endAngle = M_PI*2.0f;
    float step = (endAngle-startAngle)/float(circle_resolution);

    //todo; reserve data
    for (int i = 0; i < circle_resolution ; ++i) {
        float angle= float(i)*step;
        float angle_next = (i+1 == circle_resolution) ? endAngle : float(i+1)*step;

        float y = radius*cosf(angle);
        float z = radius*sinf(angle);

        float y_next = radius*cosf(angle_next);
        float z_next = radius*sinf(angle_next);

        data.emplace_back(top_center);
        data.emplace_back(VertexData{.pos = {half_height,y,z},.color = {r,g,b},.normal{0,0,1}});
        data.emplace_back(VertexData{.pos = {half_height,y_next,z_next},.color = {r,g,b},.normal{0,0,1}});
    }
    for (int i = 0; i < circle_resolution ; ++i) {
        float angle= float(i)*step;
        float angle_next = (i+1 == circle_resolution) ? endAngle : float(i+1)*step;

        float y = radius*cosf(angle);
        float z = radius*sinf(angle);

        float n_y = cosf(angle);
        float n_z = sinf(angle);

        float n_y_next = cosf(angle_next);
        float n_z_next = sinf(angle_next);

        float y_next = radius*cosf(angle_next);
        float z_next = radius*sinf(angle_next);

        data.emplace_back(VertexData{.pos = {half_height,y,z},.color = {r,g,b},.normal{0,n_y,n_z}});
        data.emplace_back(VertexData{.pos = {-half_height,y,z},.color = {r,g,b},.normal{0,n_y,n_z}});
        data.emplace_back(VertexData{.pos = {-half_height,y_next,z_next},.color = {r,g,b},.normal{0,n_y_next,n_z_next}});

        data.emplace_back(VertexData{.pos = {-half_height,y_next,z_next},.color = {r,g,b},.normal{0,n_y_next,n_z_next}});
        data.emplace_back(VertexData{.pos = {half_height,y_next,z_next},.color = {r,g,b},.normal{0,n_y_next,n_z_next}});
        data.emplace_back(VertexData{.pos = {half_height,y,z},.color = {r,g,b},.normal{0,n_y,n_z}});
    }
    for (int i = 0; i < circle_resolution ; ++i) {
        float angle= float(i)*step;
        float angle_next = (i+1 == circle_resolution) ? endAngle : float(i+1)*step;

        float y = radius*cosf(angle);
        float z = radius*sinf(angle);

        float y_next = radius*cosf(angle_next);
        float z_next = radius*sinf(angle_next);

        data.emplace_back(bottom_center);
        data.emplace_back(VertexData{.pos = {-half_height,y_next,z_next},.color = {r,g,b},.normal{0,0,-1}});
        data.emplace_back(VertexData{.pos = {-half_height,y,z},.color = {r,g,b},.normal{0,0,-1}});

    }
}

void VisualizerCylinder::init(QOpenGLShaderProgram *shader) {
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

void VisualizerCylinder::draw(QOpenGLShaderProgram *shader, QOpenGLFunctions *gl,QMatrix4x4 model) {
    vbo.bind();
    vao.bind();
    //we use the identity matrix,as the lines are already drawn in world coordinates

    shader->setUniformValue("model",model);

    gl->glDrawArrays(GL_TRIANGLES,0,data.size());
    vao.release();
    vbo.release();

}
