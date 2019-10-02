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
                  float _height,
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
    const float radius;
    const float startAngle;
    const float endAngle;
    const float height;
    const float wheelAngle0;
    const float wheelAngle1;
    const float wheelAngle2;
    const float wheelAngle3;
    const float wheelCenterRadius;
    const float wheelCenterHeight;
    const float wheelRadius;
    const float wheelThickness;
    const float bottomPlateHeight;
    const float kickerWidth;
    const float kickerThickness;
    const float kickerHeight;
    const float kickerGroundDist;

    //physics
    const float bodyMass;//(kg) mass of the robot not counting wheels
    const float wheelMass;//(kg) mass of a single wheel
    //TODO:add more physics parameters such as various frictions and restitutions
};


#endif //ROBOTEAM_MIMIR_ROBOTSETTINGS_H
