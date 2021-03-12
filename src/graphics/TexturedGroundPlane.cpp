//
// Created by rolf on 03-03-21.
//

#include "TexturedGroundPlane.h"
#include <QFile>

TexturedGroundPlane::TexturedGroundPlane(float square_side_length, float r, float g, float b) {
  VertexData top_right{.pos ={square_side_length,square_side_length,0},.color = {r,g,b},.normal{0.0,0.0,1.0},.texcoord{0.0,0.0}};
  VertexData top_left{.pos ={-square_side_length,square_side_length,0},.color = {r,g,b},.normal{0.0,0.0,1.0},.texcoord{1.0,0.0}};
  VertexData bottom_left{.pos ={-square_side_length,-square_side_length,0},.color = {r,g,b},.normal{0.0,0.0,1.0},.texcoord{1.0,1.0}};
  VertexData bottom_right{.pos ={square_side_length,-square_side_length,0},.color = {r,g,b},.normal{0.0,0.0,1.0},.texcoord{0.0,1.0}};

  vertices.emplace_back(top_right);
  vertices.emplace_back(top_left);
  vertices.emplace_back(bottom_right);

  vertices.emplace_back(top_left);
  vertices.emplace_back(bottom_left);
  vertices.emplace_back(bottom_right);

  ground_material = Material(QVector3D(0.01,0.035,0.0),
                             QVector3D(0.1,0.35,0.1),
                             QVector3D(0.01,0.01,0.01),
                             1.0);
}
void TexturedGroundPlane::init(QOpenGLShaderProgram *shader) {
//  QString file("/home/rolf/roboteamtwente/roboteam_mimir/src/graphics/grass.jpg"); //TOOD: clean up pathing
//  ground_texture = new QOpenGLTexture(QImage(file));
//  ground_texture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
//  ground_texture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
//  ground_texture->setWrapMode(QOpenGLTexture::Repeat);

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
  offset += sizeof(float)*3;

  int texpos_location = shader->attributeLocation("tex_pos");
  shader->enableAttributeArray(texpos_location);
  shader->setAttributeBuffer(texpos_location,GL_FLOAT,offset,2,sizeof(VertexData));

  vbo.allocate(&vertices.front(),vertices.size()*sizeof(VertexData));

  vao.release();
  vbo.release();


}
void TexturedGroundPlane::draw(QOpenGLShaderProgram *shader, QOpenGLFunctions *gl) {
  vbo.bind();
  vao.bind();
  //we use the identity matrix,as the lines are already drawn in world coordinates

  QMatrix4x4 mat;
  mat.setToIdentity();

  ground_material.bind(shader);
  //ground_texture->bind();
  shader->setUniformValue("model",mat);
  gl->glDrawArrays(GL_TRIANGLES,0,vertices.size());
  //ground_texture->release();
  vao.release();
  vbo.release();
}
TexturedGroundPlane::~TexturedGroundPlane() {
  delete ground_texture;
}
