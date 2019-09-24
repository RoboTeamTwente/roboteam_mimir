//
// Created by rolf on 24-09-19.
//

#ifndef ROBOTEAM_MIMIR_DEBUGVISUALIZATION_H
#define ROBOTEAM_MIMIR_DEBUGVISUALIZATION_H
#include <QOpenGLWidget>
namespace interface{
    class DebugVisualization : public QOpenGLWidget {
    public:
        explicit DebugVisualization(QWidget* parent = nullptr);
    };
}



#endif //ROBOTEAM_MIMIR_DEBUGVISUALIZATION_H
