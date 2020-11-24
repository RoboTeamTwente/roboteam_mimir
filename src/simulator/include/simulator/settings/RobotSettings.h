//
// Created by rolf on 27-09-19.
//

#ifndef ROBOTEAM_MIMIR_ROBOTSETTINGS_H
#define ROBOTEAM_MIMIR_ROBOTSETTINGS_H


class RobotSettings {
public:
    RobotSettings(float _radius,
                  float _startAngle,
                  float _endAngle,
                  float _totalHeight,
                  float _wheelAngle0,
                  float _wheelAngle1,
                  float _wheelAngle2,
                  float _wheelAngle3,
                  float _wheelCenterRadius,
                  float _wheelCenterHeight,
                  float _wheelRadius,
                  float _wheelThickness,
                  float _bottomPlateHeight,
                  float _kickerWidth,
                  float _kickerThickness,
                  float _kickerHeight,
                  float _kickerGroundDist,
                  float _bodyMass,
                  float _wheelMass
                  );
    float radius;
    float startAngle;
    float endAngle;
    float totalHeight;
    float wheelAngle0;
    float wheelAngle1;
    float wheelAngle2;
    float wheelAngle3;
    float wheelCenterRadius;
    float wheelCenterHeight;
    float wheelRadius;
    float wheelThickness;
    float bottomPlateHeight;
    float kickerWidth;
    float kickerThickness;
    float kickerHeight;
    float kickerGroundDist;
    float bodyMass;//(kg) mass of the robot not counting wheels
    float wheelMass;//(kg) mass of a single wheel
    //TODO:add more physics parameters such as various frictions and restitutions
};


#endif //ROBOTEAM_MIMIR_ROBOTSETTINGS_H
