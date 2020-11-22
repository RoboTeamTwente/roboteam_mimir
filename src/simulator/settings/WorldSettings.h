//
// Created by rolf on 02-10-19.
//

#ifndef ROBOTEAM_MIMIR_WORLDSETTINGS_H
#define ROBOTEAM_MIMIR_WORLDSETTINGS_H


#include <vector>

//Note everything is in mm!
struct CameraSettings {
    CameraSettings(int camID, double focalLength, double principalPointX, double principalPointY,
                   double distortion,
                   double q0, double q1, double q2, double q3,
                   double tx, double ty, double tz,
                   double derivedTX, double derivedTY, double derivedTZ,
                   int xResolution, int yResolution);
    int cameraID;
    double focalLength;
    double principalPointX;
    double principalPointY;
    double distortion;
    double q0;
    double q1;
    double q2;
    double q3;
    double tx;
    double ty;
    double tz;
    double derivedTX;
    double derivedTY;
    double derivedTZ;
    int camResolutionX;
    int camResolutionY;
};

struct WorldSettings {
    WorldSettings(
            float _fieldLength,
            float _fieldWidth,
            float _boundaryWidth,
            float _lineWidth,
            float _ceilingHeight,
            float _goalWidth,
            float _goalWallThickness,
            float _goalDepth,
            float _goalHeight,
            float _ballRadius,
            float _ballMass,
            float _gravityX,
            float _gravityY,
            float _gravityZ,
            float _centerCircleRadius,
            float scale,
            std::vector<CameraSettings> _cameras
    );
    float fieldLength;
    float fieldWidth;
    float boundaryWidth;
    float lineWidth;
    float ceilingHeight;
    float goalWidth;
    float goalWallThickness;
    float goalDepth;
    float goalHeight;
    float ballRadius;
    float ballMass;
    float gravityX;
    float gravityY;
    float gravityZ;
    float centerCircleRadius;
    float scale;
    std::vector<CameraSettings> cameras;
};


#endif //ROBOTEAM_MIMIR_WORLDSETTINGS_H
