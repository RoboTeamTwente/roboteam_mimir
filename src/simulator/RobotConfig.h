//
// Created by rolf on 29-09-19.
//

#ifndef ROBOTEAM_MIMIR_ROBOTCONFIG_H
#define ROBOTEAM_MIMIR_ROBOTCONFIG_H

#include <QSettings>
//Robot layout
//note that 0/360 degrees is the angle at which the vision pattern is detected by the simulator.
//the vision pattern coordinates are assumed to be perfectly centered in the middle of a robot.
const QString radiusStr("Layout/radius");//(m)
const QString startAngleStr("Layout/height"); //(degrees) Angle at which the arced hull starts
const QString endAngleStr("Layout/startAngle"); //(degrees) Angle at which the arced hull ends
const QString heightStr("Layout/endAngle");// total height of robot measured from the ground
const QString wheelAngle0Str("Layout/wheelAngle0");//(degrees)
const QString wheelAngle1Str("Layout/wheelAngle1");//(degrees)
const QString wheelAngle2Str("Layout/wheelAngle2");//(degrees)
const QString wheelAngle3Str("Layout/wheelAngle3");//(degrees)
const QString wheelCenterRadiusStr("Layout/wheelCentreRadius");//(m) Correction for the centre of the wheels relative to the radius
const QString wheelCenterHeightStr("Layout/wheelCentreHeight");//(m) correction for the centre of the wheels relative to the bottom plate height.
const QString wheelRadiusStr("Layout/wheelRadius");//(m)
const QString wheelThicknessStr("Layout/wheelThickness");//(m)
const QString bottomPlateHeightStr("Layout/bottomPlateHeight");//(m) total distance between the ground and the bottom of the bottom plate
const QString kickerWidthStr("Layout/kickerWidth");//(m) width of the kicker/dribbler
const QString kickerThicknessStr("Layout/kickerThickness");//(m) 'depth' of the kicker
const QString kickerHeightStr("Layout/kickerHeight");//(m) height measured from bottom to top of kicker
const QString kickerGroundDistStr("Layout/kickerGroundDist");//(m) distance between ground and the bottom of the kicker


class RobotConfig {
public:
    explicit RobotConfig(const QString &filepath);
    ~RobotConfig();
    float get(const QString &valueString) const;
    QString name() const;
private:
    QSettings *settings;

};


#endif //ROBOTEAM_MIMIR_ROBOTCONFIG_H
