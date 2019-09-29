//
// Created by rolf on 29-09-19.
//
#include "WorldConfig.h"
#include <QFileInfo>
#include <iostream>
// everything is in SI units (meters, kg's etc.)
static const QMap<QString,float> defaultValue={
        {fieldLengthStr,12.0f},
        {fieldWidthStr,9.0f},
        {boundaryWidthStr,0.3f},
        {lineWidthStr,0.01f},
        {ceilingHeightStr,5.0f},
        {goalWidthStr,1.2f},
        {goalWallThicknessStr,0.02f},
        {goalDepthStr,0.18f},
        {goalHeightStr,0.16f},
        {ballRadiusStr,0.043f},
        {ballMassStr,0.046},
        {gravityXStr,0.0f},
        {gravityYStr,0.0f},
        {gravityZStr,-9.81f}
};

WorldConfig::WorldConfig(const QString& path) {
    settings= new QSettings(path,QSettings::IniFormat);
}
WorldConfig::~WorldConfig() {
    delete settings;
}
QString WorldConfig::name() const {
    if(settings){
        return QFileInfo(settings->fileName()).baseName(); //return name without path or extensions
    }
    return "";
}
float WorldConfig::get(const QString &valueString) const {
    return settings->value(valueString,defaultValue[valueString]).toFloat();
}