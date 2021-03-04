//
// Created by rolf on 03-03-21.
//

#include "TexturedGroundPlane.h"
TexturedGroundPlane::TexturedGroundPlane(float square_side_length, float r, float g, float b) {
  VertexData top_right{.pos ={square_side_length,square_side_length,0},.color = {r,g,b}};
  VertexData top_left{.pos ={-square_side_length,square_side_length,0},.color = {r,g,b}};
  VertexData bottom_left{.pos ={-square_side_length,-square_side_length,0},.color = {r,g,b}};
  VertexData bottom_right{.pos ={square_side_length,-square_side_length,0},.color = {r,g,b}};

  vertices.emplace_back(top_right);
  vertices.emplace_back(top_left);
  vertices.emplace_back(bottom_right);

  vertices.emplace_back(top_left);
  vertices.emplace_back(bottom_left);
  vertices.emplace_back(bottom_right);
}
void TexturedGroundPlane::init(QOpenGLShaderProgram *shader) {
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

  shader->setUniformValue("model",mat);

  gl->glDrawArrays(GL_TRIANGLES,0,vertices.size());
  vao.release();
  vbo.release();
}
