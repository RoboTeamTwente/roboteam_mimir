//
// Created by rolf on 03-03-21.
//

#ifndef ROBOTEAM_MIMIR_SRC_GRAPHICS_TEXTUREDGROUNDPLANE_H_
#define ROBOTEAM_MIMIR_SRC_GRAPHICS_TEXTUREDGROUNDPLANE_H_
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include "Material.h"

class TexturedGroundPlane {
 public:
  TexturedGroundPlane(float square_side_length,float r, float g, float b);
  ~TexturedGroundPlane();
  void init(QOpenGLShaderProgram * shader);
  void draw(QOpenGLShaderProgram * shader, QOpenGLFunctions * gl);
 private:
  struct VertexData{
    float pos[3];
    float color[3];
    float normal[3];
    float texcoord[2];
  };
  Material ground_material;
  std::vector<VertexData> vertices;

  QOpenGLVertexArrayObject vao;
  QOpenGLBuffer vbo;
  QOpenGLTexture * ground_texture;
};

#endif //ROBOTEAM_MIMIR_SRC_GRAPHICS_TEXTUREDGROUNDPLANE_H_
