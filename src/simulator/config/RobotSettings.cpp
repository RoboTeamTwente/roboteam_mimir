//
// Created by rolf on 02-10-19.
//

#include "RobotSettings.h"
RobotSettings::RobotSettings(float _radius, float _height,float _startAngle, float _endAngle, float _wheelAngle0,
                             float _wheelAngle1, float _wheelAngle2, float _wheelAngle3, float _wheelCenterRadius,
                             float _wheelCenterHeight, float _wheelRadius, float _wheelThickness,
                             float _bottomPlateHeight, float _kickerWidth, float _kickerThickness, float _kickerHeight,
                             float _kickerGroundDist, float _bodyMass, float _wheelMass):
                             radius(_radius),
                             startAngle(_startAngle),
                             endAngle(_endAngle),
                             height(_height),
                             wheelAngle0(_wheelAngle0),
                             wheelAngle1(_wheelAngle1),
                             wheelAngle2(_wheelAngle2),
                             wheelAngle3(_wheelAngle3),
                             wheelCenterRadius(_wheelCenterRadius),
                             wheelCenterHeight(_wheelCenterHeight),
                             wheelRadius(_wheelRadius),
                             wheelThickness(_wheelThickness),
                             bottomPlateHeight(_bottomPlateHeight),
                             kickerWidth(_kickerWidth),
                             kickerThickness(_kickerThickness),
                             kickerHeight(_kickerHeight),
                             kickerGroundDist(_kickerGroundDist),
                             bodyMass(_bodyMass),
                             wheelMass(_wheelMass)
                             {}