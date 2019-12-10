//
// Created by rolf on 29-09-19.
//

#include "RobotConfig.h"
#include <QFileInfo>
#include <iostream>

//put string variables for interpretation in anonymous namespace
namespace {
    //Robot layout
//note that 0/360 degrees is the angle at which the vision pattern is detected by the simulator.
//(so if you have an offset you need to compensate for it)
//the vision pattern coordinates are assumed to be perfectly centered in the middle of a robot. (as is the case for SSL-vision too)

    const QString radiusStr("Layout/radius");//(m)
    const QString startAngleStr("Layout/totalHeight"); //(degrees) Angle at which the arced hull starts
    const QString endAngleStr("Layout/startAngle"); //(degrees) Angle at which the arced hull ends
    const QString totalHeightStr("Layout/endAngle");// total height of robot measured from the ground
    const QString wheelAngle0Str("Layout/wheelAngle0");//(degrees)
    const QString wheelAngle1Str("Layout/wheelAngle1");//(degrees)
    const QString wheelAngle2Str("Layout/wheelAngle2");//(degrees)
    const QString wheelAngle3Str("Layout/wheelAngle3");//(degrees)
    const QString wheelCenterRadiusStr(
            "Layout/wheelCentreRadius");//(m) Correction for the centre of the wheels relative to the radius
    const QString wheelCenterHeightStr(
            "Layout/wheelCentreHeight");//(m) correction for the centre of the wheels relative to the bottom plate height.
    const QString wheelRadiusStr("Layout/wheelRadius");//(m)
    const QString wheelThicknessStr("Layout/wheelThickness");//(m)
    const QString bottomPlateHeightStr(
            "Layout/bottomPlateHeight");//(m) total distance between the ground and the bottom of the bottom plate
    const QString kickerWidthStr("Layout/kickerWidth");//(m) width of the kicker/dribbler
    const QString kickerThicknessStr("Layout/kickerThickness");//(m) 'depth' of the kicker
    const QString kickerHeightStr("Layout/kickerHeight");//(m) height measured from bottom to top of kicker
    const QString kickerGroundDistStr(
            "Layout/kickerGroundDist");//(m) distance between ground and the bottom of the kicker
    const QString bodyMassStr("Physics/bodyMass");//(kg) mass of the body/hull
    const QString wheelMassStr("Physics/wheelMass");//(kg) mass of a wheel
    const QMap<QString, float> defaultRobotValue = {
            {radiusStr,            0.09f},
            {startAngleStr,        45.0f},
            {endAngleStr,          315.0f},
            {totalHeightStr,       0.15f},
            {wheelAngle0Str,       60.0f},
            {wheelAngle1Str,       135.0f},
            {wheelAngle2Str,       225.0f},
            {wheelAngle3Str,       300.0f},
            {wheelCenterRadiusStr, 0.0f},
            {wheelCenterHeightStr, 0.0f},
            {wheelRadiusStr,       0.028f},
            {wheelThicknessStr,    0.011f},
            {bottomPlateHeightStr, 0.005f},
            {kickerWidthStr,       0.075f},
            {kickerThicknessStr,   0.01},
            {kickerHeightStr,      0.008},
            {kickerGroundDistStr,  0.025},
            {bodyMassStr,          2.0},
            {wheelMassStr,         0.05}
    };
}
RobotConfig::RobotConfig(const QString &path) {
    settingsFile = std::make_unique<QSettings>(path, QSettings::IniFormat);
    settings =  std::make_unique<RobotSettings>(
            get(radiusStr),
            get(startAngleStr),
            get(endAngleStr),
            get(totalHeightStr),
            get(wheelAngle0Str),
            get(wheelAngle1Str),
            get(wheelAngle2Str),
            get(wheelAngle3Str),
            get(wheelCenterRadiusStr),
            get(wheelCenterHeightStr),
            get(wheelRadiusStr),
            get(wheelThicknessStr),
            get(bottomPlateHeightStr),
            get(kickerWidthStr),
            get(kickerThicknessStr),
            get(kickerHeightStr),
            get(kickerGroundDistStr),
            get(bodyMassStr),
            get(wheelMassStr)
    );
}
QString RobotConfig::name() const {
    if (settingsFile) {
        return QFileInfo(settingsFile->fileName()).baseName(); //return name without path or extensions
    }
    return "";
}
float RobotConfig::get(const QString &valueString) const {
    if (settingsFile->contains(valueString)) {
        return settingsFile->value(valueString).toFloat();
    }
    std::cerr << "Could not find " << valueString.toStdString() << " in RobotConfig " << name().toStdString()
              << ", returning default" << std::endl;
    return defaultRobotValue[valueString];
}