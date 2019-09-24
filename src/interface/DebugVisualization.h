//
// Created by rolf on 24-09-19.
//

#ifndef ROBOTEAM_MIMIR_DEBUGVISUALIZATION_H
#define ROBOTEAM_MIMIR_DEBUGVISUALIZATION_H

#include <QOpenGLWidget>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLShader>
#include <QtGui/QOpenGLBuffer>

namespace interface {
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
        QOpenGLVertexArrayObject vao;
        QOpenGLBuffer vbo;
        QOpenGLShader *shader;
        QOpenGLShaderProgram shaderProgram;

    };
}


#endif //ROBOTEAM_MIMIR_DEBUGVISUALIZATION_H
