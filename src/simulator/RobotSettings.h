//
// Created by rolf on 27-09-19.
//

#ifndef ROBOTEAM_MIMIR_ROBOTSETTINGS_H
#define ROBOTEAM_MIMIR_ROBOTSETTINGS_H


struct RobotSettings {
    //Robot layout
    //note that 0/360 degrees is the angle at which the vision pattern is detected by the simulator.
    //the vision pattern coordinates are assumed to be perfectly centered in the middle of a robot.
    float radius;//(m) radius of the robot's hull
    float startAngle; //(degrees) Angle at which the arced hull starts
    float endAngle; //(degrees) Angle at which the arced hull ends
    float height;// total height of robot measured from the ground
    float wheelAngle0;//(degrees)
    float wheelAngle1;//(degrees)
    float wheelAngle2;//(degrees)
    float wheelAngle3;//(degrees)
    float wheelCenterRadius;//(m) Correction for the centre of the wheels relative to the radius
    float wheelCenterHeight;//(m) correction for the centre of the wheels relative to the bottom plate height.
    float wheelRadius;//(m)
    float wheelThickness;//(m)
    float bottomPlateHeight;//(m) total distance between the ground and the bottom of the bottom plate
    float kickerWidth;//(m) width of the kicker/dribbler
    float kickerThickness;//(m) 'depth' of the kicker
    float kickerHeight;//(m) height measured from bottom to top of kicker
    float kickerGroundDist;//(m) distance between ground and the bottom of the kicker

    //physics
    float bodyMass;//(kg) mass of the robot not counting wheels
    float wheelMass;//(kg) mass of a single wheel
    //TODO:add more physics parameters such as various frictions and restitutions
};


#endif //ROBOTEAM_MIMIR_ROBOTSETTINGS_H
