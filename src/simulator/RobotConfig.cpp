//
// Created by rolf on 29-09-19.
//

#include "RobotConfig.h"
#include <QFileInfo>
#include <iostream>
// everything is in SI units (meters, kg's etc.)
namespace {
    const QMap<QString, float> defaultRobotValue = {
            {radiusStr, 0.09f},
            {startAngleStr,45.0f},
            {endAngleStr,315.0f},
            {heightStr,0.15f},
            {wheelAngle0Str,60.0f},
            {wheelAngle1Str,135.0f},
            {wheelAngle2Str,225.0f},
            {wheelAngle3Str,300.0f},
            {wheelCenterRadiusStr,0.0f},
            {wheelCenterHeightStr,0.0f},
            {wheelRadiusStr,0.028f},
            {wheelThicknessStr,0.011f},
            {bottomPlateHeightStr,0.005f},
            {kickerWidthStr,0.075f},
            {kickerThicknessStr,0.01},
            {kickerHeightStr,0.008},
            {kickerGroundDistStr,0.025}
    };
}
RobotConfig::RobotConfig(const QString &path) {
    settings = new QSettings(path, QSettings::IniFormat);
}
RobotConfig::~RobotConfig() {
    delete settings;
}
QString RobotConfig::name() const {
    if (settings) {
        return QFileInfo(settings->fileName()).baseName(); //return name without path or extensions
    }
    return "";
}
float RobotConfig::get(const QString &valueString) const {
    if (settings->contains(valueString)){
        return settings->value(valueString).toFloat();
    }
    std::cerr<<"Could not find " <<valueString.toStdString()<<" in RobotConfig "<<name().toStdString() <<", returning default" <<std::endl;
    return defaultRobotValue[valueString];
}