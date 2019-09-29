//
// Created by rolf on 29-09-19.
//

#ifndef ROBOTEAM_MIMIR_WORLDCONFIG_H
#define ROBOTEAM_MIMIR_WORLDCONFIG_H

#include <QSettings>

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


class WorldConfig {
public:
    explicit WorldConfig(const QString& filepath);
    ~WorldConfig();
    float get(const QString& valueString) const;
    QString name() const;
private:
    QSettings* settings;
};



#endif //ROBOTEAM_MIMIR_WORLDCONFIG_H
