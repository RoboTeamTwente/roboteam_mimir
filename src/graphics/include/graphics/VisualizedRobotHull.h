//
// Created by rolf on 11-03-21.
//

#ifndef ROBOTEAM_MIMIR_SRC_GRAPHICS_VISUALIZEDROBOTHULL_H_
#define ROBOTEAM_MIMIR_SRC_GRAPHICS_VISUALIZEDROBOTHULL_H_
#include <vector>
#include <QVector3D>
#include <simulator/SimBot.h>
#include "Material.h"
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

class VisualizedRobotHull {
 public:
  explicit VisualizedRobotHull(const RobotSettings& settings);
  ~VisualizedRobotHull();
  void init(QOpenGLShaderProgram * shader);
  void draw(QOpenGLShaderProgram * shader, QOpenGLFunctions * gl,const QMatrix4x4& mat);
 private:
  static QVector3D toQVec(const btVector3& btVec);
  static QVector2D toTexCoord(const btVector3& btVec);
  Material material;
  Material texture_material;
  QOpenGLVertexArrayObject vao;

  QOpenGLBuffer vbo;


  struct VertexData{
    QVector3D pos;
    QVector3D color;
    QVector3D normal;
    QVector2D tex_coord;
  };
  std::vector<VertexData> data;

  QOpenGLBuffer top_texture_buffer;
  QOpenGLVertexArrayObject top_vao;

  std::vector<VertexData> top_buffer;
  QOpenGLTexture * top_texture;
};

#endif //ROBOTEAM_MIMIR_SRC_GRAPHICS_VISUALIZEDROBOTHULL_H_
