//
// Created by rolf on 02-10-19.
//

#ifndef ROBOTEAM_MIMIR_WORLDSETTINGS_H
#define ROBOTEAM_MIMIR_WORLDSETTINGS_H


#include <vector>

//Note everything is in mm!
class CameraSettings {
public:
    CameraSettings(int camID, double focalLength, double principalPointX, double principalPointY,
                   double distortion,
                   double q0, double q1, double q2, double q3,
                   double tx, double ty, double tz,
                   double derivedTX, double derivedTY, double derivedTZ,
                   int xResolution, int yResolution);
    public:
        int cameraID() const;
        double focalLength() const;
        double principalPointX() const;
        double principalPointY() const;
        double distortion() const;
        double q0() const;
        double q1() const;
        double q2() const;
        double q3() const;
        double tx() const;
        double ty() const;
        double tz() const;
        double derivedTX() const;
        double derivedTY() const;
        double derivedTZ() const;
        int camResolutionX() const;
        int camResolutionY() const;
    private:
    int _cameraID;
    double _focalLength;
    double _principalPointX;
    double _principalPointY;
    double _distortion;
    double _q0;
    double _q1;
    double _q2;
    double _q3;
    double _tx;
    double _ty;
    double _tz;
    double _derivedTX;
    double _derivedTY;
    double _derivedTZ;
    int _camResolutionX;
    int _camResolutionY;
};

class WorldSettings {
public:
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
    public:
        float fieldLength() const;
        float fieldWidth() const;
        float boundaryWidth() const;
        float lineWidth() const;
        float ceilingHeight() const;
        float goalWidth() const;
        float goalWallThickness() const;
        float goalDepth() const;
        float goalHeight() const;
        float ballRadius() const;
        float ballMass() const;
        float gravityX() const;
        float gravityY() const;
        float gravityZ() const;
        float centerCircleRadius() const;
        float scale() const;
        const std::vector<CameraSettings>& cameras() const;
    private:

    float _fieldLength;
    float _fieldWidth;
    float _boundaryWidth;
    float _lineWidth;
    float _ceilingHeight;
    float _goalWidth;
    float _goalWallThickness;
    float _goalDepth;
    float _goalHeight;
    float _ballRadius;
    float _ballMass;
    float _gravityX;
    float _gravityY;
    float _gravityZ;
    float _centerCircleRadius;
    float _scale;
    std::vector<CameraSettings> _cameras;

};


#endif //ROBOTEAM_MIMIR_WORLDSETTINGS_H
