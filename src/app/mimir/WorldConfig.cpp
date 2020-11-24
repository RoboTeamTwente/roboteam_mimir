//
// Created by rolf on 29-09-19.
//
#include "WorldConfig.h"
#include <QFileInfo>
#include <iostream>
//anonymous namespace to keep these variables out of global scope
namespace {
    const QString fieldLengthStr("Geometry/fieldLength");
    const QString fieldWidthStr("Geometry/fieldWidth");
    const QString boundaryWidthStr("Geometry/boundaryWidth");
    const QString lineWidthStr("Geometry/lineWidth");
    const QString ceilingHeightStr("Geometry/ceilingHeight");
    const QString goalWidthStr("Geometry/goalWidth");
    const QString goalWallThicknessStr("Geometry/goalWallThickness");
    const QString goalDepthStr("Geometry/goalDepth");
    const QString goalHeightStr("Geometry/goalHeight");
    const QString ballRadiusStr("Ball/ballRadius");
    const QString ballMassStr("Ball/ballMass");
    const QString gravityXStr("Physics/gravityX");
    const QString gravityYStr("Physics/gravityY");
    const QString gravityZStr("Physics/gravityZ");
    const QString centerCircleRadiusStr("Geometry/centerCircleRadius");
    const QString scaleStr("Physics/scale");

    const QMap<QString, float> defaultWorldValue = {
            {fieldLengthStr,        12.0f},
            {fieldWidthStr,         9.0f},
            {boundaryWidthStr,      0.3f},
            {lineWidthStr,          0.01f},
            {ceilingHeightStr,      5.0f},
            {goalWidthStr,          1.2f},
            {goalWallThicknessStr,  0.02f},
            {goalDepthStr,          0.18f},
            {goalHeightStr,         0.16f},
            {ballRadiusStr,         0.043f},
            {ballMassStr,           0.046},
            {gravityXStr,           0.0f},
            {gravityYStr,           0.0f},
            {gravityZStr,           -9.81f},
            {centerCircleRadiusStr, 1.0f},
            {scaleStr,              1.0}
    };
}

WorldConfig::WorldConfig(const QString &path) {
    settingsFile = std::make_unique<QSettings>(path, QSettings::IniFormat);
    std::vector<CameraSettings> cameras;
    int camCount = settingsFile->beginReadArray(QString("Camera"));
    for (int i = 0; i < camCount; ++i) {
        settingsFile->setArrayIndex(i);
        double focalLength = settingsFile->value(QString("focalLength")).toDouble();
        double principalPointX = settingsFile->value(QString("principalPointX")).toDouble();
        double principalPointY = settingsFile->value(QString("principalPointY")).toDouble();
        double distortion = settingsFile->value(QString("distortion")).toDouble();
        double q0 = settingsFile->value(QString("q0")).toDouble();
        double q1 = settingsFile->value(QString("q1")).toDouble();
        double q2 = settingsFile->value(QString("q2")).toDouble();
        double q3 = settingsFile->value(QString("q3")).toDouble();
        double tx = settingsFile->value(QString("tx")).toDouble();
        double ty = settingsFile->value(QString("ty")).toDouble();
        double tz = settingsFile->value(QString("tz")).toDouble();
        double derivedtx = settingsFile->value(QString("derivedtx")).toDouble();
        double derivedty = settingsFile->value(QString("derivedty")).toDouble();
        double derivedtz = settingsFile->value(QString("derivedtz")).toDouble();
        int camID = settingsFile->value(QString("camID")).toInt();
        int xRes = settingsFile->value(QString("xRes"), 0).toInt();
        int yRes = settingsFile->value(QString("yRes"), 0).toInt();
        cameras.emplace_back(
                CameraSettings(camID, focalLength, principalPointX, principalPointY, distortion, q0, q1, q2, q3, tx, ty,
                               tz, derivedtx, derivedty, derivedtz, xRes, yRes));

    }
    settingsFile->endArray();

    settings = std::make_unique<WorldSettings>(
            get(fieldLengthStr),
            get(fieldWidthStr),
            get(boundaryWidthStr),
            get(lineWidthStr),
            get(ceilingHeightStr),
            get(goalWidthStr),
            get(goalWallThicknessStr),
            get(goalDepthStr),
            get(goalHeightStr),
            get(ballRadiusStr),
            get(ballMassStr),
            get(gravityXStr),
            get(gravityYStr),
            get(gravityZStr),
            get(centerCircleRadiusStr),
            get(scaleStr),
            cameras
    );
}
QString WorldConfig::name() const {
    if (settingsFile) {
        return QFileInfo(settingsFile->fileName()).baseName(); //return name without path or extensions
    }
    return "";
}
float WorldConfig::get(const QString &valueString) const {
    if (settingsFile->contains(valueString)) {
        return settingsFile->value(valueString).toFloat(); // Every setting should be a float!
    }
    std::cerr << "Could not find " << valueString.toStdString() << " in WorldConfig " << name().toStdString()
              << ", returning default" << std::endl;
    return defaultWorldValue[valueString];
}