//
// Created by rolf on 27-02-21.
//

#ifndef ROBOTEAM_MIMIR_VISUALIZERWORLD_H
#define ROBOTEAM_MIMIR_VISUALIZERWORLD_H

#include "VisualizerBox.h"
#include "VisualizerBall.h"
#include "VisualizerCylinder.h"
class VisualizerWorld {
public:
    VisualizerWorld();
    void init(QOpenGLShaderProgram * shader);
    void draw(QOpenGLShaderProgram * shader, QOpenGLFunctions * gl);
private:
    VisualizerBox * box;
    VisualizerBall * ball;
    VisualizerCylinder * cylinder;
};


#endif //ROBOTEAM_MIMIR_VISUALIZERWORLD_H
