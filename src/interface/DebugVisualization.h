//
// Created by rolf on 24-09-19.
//

#ifndef ROBOTEAM_MIMIR_DEBUGVISUALIZATION_H
#define ROBOTEAM_MIMIR_DEBUGVISUALIZATION_H

#include <QOpenGLWidget>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLShader>
#include <QtGui/QOpenGLBuffer>
#include <QOpenGLFunctions>
namespace interface {
    struct VertexData{
        QVector3D pos;
        QVector4D color;// QColor does not go well with OpenGl so we use QVector4D to represent RGBa
    };
    class DebugVisualization : public QOpenGLWidget {
    Q_OBJECT
    public:
        explicit DebugVisualization(QWidget *parent = nullptr);
        ~DebugVisualization();
    protected:
        void initializeGL() override;
        void resizeGL(int w, int h) override;
        void paintGL() override;
    private:
        void addLine(const QVector3D &p1,QVector3D &p2,const QVector4D color);
        void draw(QOpenGLFunctions *f);
        void setupShaders();
        QString findShaderDir();
        QOpenGLVertexArrayObject vao;
        QOpenGLBuffer vbo;
        QOpenGLShader *shader;
        QOpenGLShaderProgram shaderProgram;
        std::vector<VertexData> lines;

    };
}


#endif //ROBOTEAM_MIMIR_DEBUGVISUALIZATION_H
