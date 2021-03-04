//
// Created by rolf on 27-02-21.
//

#include <cmath>
#include "VisualizerBall.h"

void VisualizerBall::init(QOpenGLShaderProgram *shader) {
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

void VisualizerBall::draw(QOpenGLShaderProgram *shader, QOpenGLFunctions *gl, const QMatrix4x4& modelToWorld) {
    vbo.bind();
    vao.bind();
    //we use the identity matrix,as the lines are already drawn in world coordinates

    shader->setUniformValue("model",modelToWorld);

    gl->glDrawArrays(GL_TRIANGLES,0,data.size());
    vao.release();
    vbo.release();
}

VisualizerBall::VisualizerBall(float radius, float r, float g, float b) {
    int u_res = 20;
    int v_res = 20;


    float startU = 0.0;
    float startV = 0.0;
    float endU = M_PI*2;
    float endV = M_PI;

    float stepU = (endU-startU)/float(u_res);
    float stepV = (endV-startV)/float(v_res);

    for (int i = 0; i < u_res ; ++i) {
        for (int j = 0; j < v_res; ++j) {
            float u = startU + float(i)*stepU;
            float v= startV + float(j)*stepV;
            float u_next = (i+1 == u_res) ? endU : float(i+1)*stepU + startU;
            float v_next = (j+1 == v_res) ? endV : float(j+1)*stepV + startV;

            float pos_1[3];
            float pos_2[3];
            float pos_3[3];
            float pos_4[4];
            sphereEQ(u,v,radius,pos_1);
            sphereEQ(u,v_next,radius,pos_2);
            sphereEQ(u_next,v,radius,pos_3);
            sphereEQ(u_next,v_next,radius,pos_4);

            addTriangle(pos_1,pos_3,pos_2,r,g,b);
            addTriangle(pos_4,pos_2,pos_3,r,g,b);

        }
    }
}

void VisualizerBall::sphereEQ(float u, float v, float radius, float *write_to) {
    write_to[0] = cosf(u)*sinf(v)*radius;
    write_to[1] = cosf(v)*radius;
    write_to[2] = sinf(u)*sinf(v)*radius;
}

void VisualizerBall::addTriangle(float *a, float *b, float *c, float r, float g, float blue) {
    data.emplace_back(VertexData{.pos = {a[0],a[1],a[2]},.color ={r,g,blue}});
    data.emplace_back(VertexData{.pos = {b[0],b[1],b[2]},.color ={r,g,blue}});
    data.emplace_back(VertexData{.pos = {c[0],c[1],c[2]},.color ={r,g,blue}});

}
VisualizerBall::~VisualizerBall() {
  double test = 1;

}
