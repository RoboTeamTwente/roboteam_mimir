//
// Created by rolf on 02-10-19.
//

#include "RobotSettings.h"
RobotSettings::RobotSettings(float _radius, float _totalHeight,float _startAngle, float _endAngle, float _wheelAngle0,
                             float _wheelAngle1, float _wheelAngle2, float _wheelAngle3, float _wheelCenterRadius,
                             float _wheelCenterHeight, float _wheelRadius, float _wheelThickness,
                             float _bottomPlateHeight, float _kickerWidth, float _kickerThickness, float _kickerHeight,
                             float _kickerGroundDist, float _bodyMass, float _wheelMass):
                             _radius(_radius),
                             _startAngle(_startAngle),
                             _endAngle(_endAngle),
                             _totalHeight(_totalHeight),
                             _wheelAngle0(_wheelAngle0),
                             _wheelAngle1(_wheelAngle1),
                             _wheelAngle2(_wheelAngle2),
                             _wheelAngle3(_wheelAngle3),
                             _wheelCenterRadius(_wheelCenterRadius),
                             _wheelCenterHeight(_wheelCenterHeight),
                             _wheelRadius(_wheelRadius),
                             _wheelThickness(_wheelThickness),
                             _bottomPlateHeight(_bottomPlateHeight),
                             _kickerWidth(_kickerWidth),
                             _kickerThickness(_kickerThickness),
                             _kickerHeight(_kickerHeight),
                             _kickerGroundDist(_kickerGroundDist),
                             _bodyMass(_bodyMass),
                             _wheelMass(_wheelMass)
                             {}
float RobotSettings::radius() {
    return _radius;
}
float RobotSettings::startAngle() {
    return _startAngle;
}
float RobotSettings::endAngle() {
    return _endAngle;
}
float RobotSettings::totalHeight() {
    return _totalHeight;
}
float RobotSettings::wheelAngle0() {
    return _wheelAngle0;
}
float RobotSettings::wheelAngle1() {
    return _wheelAngle1;
}
float RobotSettings::wheelAngle2() {
    return _wheelAngle2;
}
float RobotSettings::wheelAngle3() {
    return _wheelAngle3;
}
float RobotSettings::wheelCenterRadius() {
    return _wheelCenterRadius;
}
float RobotSettings::wheelCenterHeight() {
    return _wheelCenterHeight;
}
float RobotSettings::wheelRadius() {
    return _wheelRadius;
}
float RobotSettings::wheelThickness() {
    return _wheelThickness;
}
float RobotSettings::bottomPlateHeight() {
    return _bottomPlateHeight;
}
float RobotSettings::kickerWidth() {
    return _kickerWidth;
}
float RobotSettings::kickerThickness() {
    return _kickerThickness;
}
float RobotSettings::kickerHeight() {
    return _kickerHeight;
}
float RobotSettings::kickerGroundDist() {
    return _kickerGroundDist;
}
float RobotSettings::bodyMass() {
    return _bodyMass;
}
float RobotSettings::wheelMass() {
    return _wheelMass;
}
