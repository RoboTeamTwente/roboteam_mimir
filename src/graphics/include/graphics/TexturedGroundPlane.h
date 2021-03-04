//
// Created by rolf on 03-03-21.
//

#ifndef ROBOTEAM_MIMIR_SRC_GRAPHICS_TEXTUREDGROUNDPLANE_H_
#define ROBOTEAM_MIMIR_SRC_GRAPHICS_TEXTUREDGROUNDPLANE_H_
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class TexturedGroundPlane {
 public:
  TexturedGroundPlane(float square_side_length,float r, float g, float b);
  void init(QOpenGLShaderProgram * shader);
  void draw(QOpenGLShaderProgram * shader, QOpenGLFunctions * gl);
 private:
  struct VertexData{
    float pos[3];
    float color[3];

  };
  std::vector<VertexData> vertices;

  QOpenGLVertexArrayObject vao;
  QOpenGLBuffer vbo;
};

#endif //ROBOTEAM_MIMIR_SRC_GRAPHICS_TEXTUREDGROUNDPLANE_H_
