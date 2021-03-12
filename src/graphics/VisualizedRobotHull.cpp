//
// Created by rolf on 11-03-21.
//

#include "VisualizedRobotHull.h"
VisualizedRobotHull::VisualizedRobotHull(const RobotSettings &settings) {

  RobotMesh mesh(settings);
  QVector3D color ={1.0,1.0,1.0};
  //draw top
  QVector3D topNormal(0,0,1.0);
  std::vector<btVector3> top_circle = mesh.topCircle();

  for (int i = 0; i < top_circle.size(); ++i) {
    data.emplace_back(VertexData{toQVec(mesh.top()),color,topNormal,toTexCoord(mesh.top())});
    data.emplace_back(VertexData{toQVec(top_circle[(i+1)%top_circle.size()]),color,topNormal,toTexCoord(top_circle[(i+1)%top_circle.size()])});
    data.emplace_back(VertexData{toQVec(top_circle[i]),color,topNormal,toTexCoord(top_circle[i])});
  }
  for (int i = 0; i < top_circle.size(); ++i) {
    top_buffer.emplace_back(VertexData{toQVec(mesh.top()),color,topNormal,toTexCoord(mesh.top())});
    top_buffer.emplace_back(VertexData{toQVec(top_circle[(i+1)%top_circle.size()]),color,topNormal,toTexCoord(top_circle[(i+1)%top_circle.size()])});
    top_buffer.emplace_back(VertexData{toQVec(top_circle[i]),color,topNormal,toTexCoord(top_circle[i])});
  }
  for(auto& elem: top_buffer){
    elem.pos.setZ(elem.pos.z()+0.0001f);
  }
  QVector3D bottomNormal(0,0,-1.0);
  std::vector<btVector3> bottom_circle = mesh.bottomCircle();
  for (int i = 0; i < bottom_circle.size(); ++i) {
    data.emplace_back(VertexData{toQVec(mesh.bottom()),color,bottomNormal});
    data.emplace_back(VertexData{toQVec(bottom_circle[i]),color,bottomNormal});
    data.emplace_back(VertexData{toQVec(bottom_circle[(i+1)%bottom_circle.size()]),color,bottomNormal});
  }

  assert(bottom_circle.size() == top_circle.size());
  int n = bottom_circle.size();
  for (int i = 0; i < n; ++i) {
    QVector3D firstNormal = toQVec(top_circle[i]);
    firstNormal.setZ(0.0);
    firstNormal.normalize();

    QVector3D secondNormal = toQVec(top_circle[(i+1)%n]);
    secondNormal.setZ(0.0);
    secondNormal.normalize();

    data.emplace_back(VertexData{toQVec(top_circle[i]),color,firstNormal});
    data.emplace_back(VertexData{toQVec(bottom_circle[(i+1)%n]),color,secondNormal});
    data.emplace_back(VertexData{toQVec(bottom_circle[i]),color,firstNormal});

    data.emplace_back(VertexData{toQVec(top_circle[(i+1)%n]),color,secondNormal});
    data.emplace_back(VertexData{toQVec(bottom_circle[(i+1)%n]),secondNormal});
    data.emplace_back(VertexData{toQVec(top_circle[i]),color,firstNormal});
  }
  material = Material(QVector3D(0.1,0.1,0.1),QVector3D(0.2,0.2,0.2),
                      QVector3D(0.5,0.5,0.5),8.0);
  texture_material = Material(QVector3D(1.0,1.0,1.0),QVector3D(1.0,1.0,1.0),
                              QVector3D(0.3,0.3,0.3),32.0);
    //
}
QVector3D VisualizedRobotHull::toQVec(const btVector3 & btVec) {
  return QVector3D(btVec.x(),btVec.y(),btVec.z());
}
void VisualizedRobotHull::init(QOpenGLShaderProgram *shader) {
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

  vbo.allocate(&data.front(),data.size()*sizeof(VertexData));




  vao.release();
  vbo.release();


  top_texture_buffer.create();
  top_texture_buffer.bind();

  top_vao.create();
  top_vao.bind();

  offset = 0;
  QString file("/home/rolf/roboteamtwente/roboteam_mimir/resources/textures/b0.png"); //TODO: clean up pathing
  top_texture = new QOpenGLTexture(QImage(file));
  top_texture->setMagnificationFilter(QOpenGLTexture::Linear);
  top_texture->setMagnificationFilter(QOpenGLTexture::Linear);
  top_texture->setWrapMode(QOpenGLTexture::Repeat);
  top_texture->setFormat(QOpenGLTexture::RGBA16F);

  pos_location = shader->attributeLocation("pos");
  shader->enableAttributeArray(pos_location);
  shader->setAttributeBuffer(pos_location,GL_FLOAT,offset,3,sizeof(VertexData));

  offset += sizeof(float)*3;
  color_location = shader->attributeLocation("color");
  shader->enableAttributeArray(color_location);
  shader->setAttributeBuffer(color_location,GL_FLOAT,offset,3,sizeof(VertexData));

  offset += sizeof(float)*3;
  normal_location = shader->attributeLocation("objectNormal");
  shader->enableAttributeArray(normal_location);
  shader->setAttributeBuffer(normal_location,GL_FLOAT,offset,3,sizeof(VertexData));
  offset += sizeof(float)*3;

  texpos_location = shader->attributeLocation("tex_pos");
  shader->enableAttributeArray(texpos_location);
  shader->setAttributeBuffer(texpos_location,GL_FLOAT,offset,2,sizeof(VertexData));

  top_texture_buffer.allocate(&top_buffer.front(),top_buffer.size()*sizeof(VertexData));

  top_vao.release();
  top_texture_buffer.release();
}
void VisualizedRobotHull::draw(QOpenGLShaderProgram *shader, QOpenGLFunctions *gl, const QMatrix4x4 &model) {
  material.bind(shader);
  vbo.bind();
  vao.bind();

  shader->setUniformValue("model",model);
  gl->glDrawArrays(GL_TRIANGLES,0,data.size());

  vao.release();
  vbo.release();

  texture_material.bind(shader);
  top_texture_buffer.bind();
  top_vao.bind();

  shader->setUniformValue("compute_texture",true);
  top_texture->bind();
  top_texture_buffer.bind();

  gl->glDrawArrays(GL_TRIANGLES,0,top_buffer.size());

  top_vao.release();
  top_texture_buffer.release();
  shader->setUniformValue("compute_texture",false);

}
QVector2D VisualizedRobotHull::toTexCoord(const btVector3 &btVec) {
  //image is (8.5cm by 8.5cm). Image x and y are swapped with robot local x and y
  float image_length = 0.085f*2;
  QVector2D local((btVec.y()+0.5f*image_length)/image_length,-(btVec.x()+0.5f*image_length)/image_length);
  return local;
}
VisualizedRobotHull::~VisualizedRobotHull() {
  delete top_texture;
}
