//
// Created by rolf on 27-09-19.
//

#ifndef ROBOTEAM_MIMIR_ROBOTSETTINGS_H
#define ROBOTEAM_MIMIR_ROBOTSETTINGS_H


struct RobotSettings {

    float radius;
    float startAngle;
    float endAngle;
    float height;
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

    //physics
    float bodyMass;//(kg) mass of the robot not counting wheels
    float wheelMass;//(kg) mass of a single wheel
    //TODO:add more physics parameters such as various frictions and restitutions
};


#endif //ROBOTEAM_MIMIR_ROBOTSETTINGS_H
