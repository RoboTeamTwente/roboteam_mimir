//
// Created by rolf on 02-10-19.
//

#include "settings/WorldSettings.h"

#include <utility>

WorldSettings::WorldSettings(float _fieldLength, float _fieldWidth, float _boundaryWidth, float _lineWidth,
                             float _ceilingHeight, float _goalWidth, float _goalWallThickness, float _goalDepth,
                             float _goalHeight, float _ballRadius, float _ballMass, float _gravityX, float _gravityY,
                             float _gravityZ, float _centerCircleRadius, float _scale, std::vector<CameraSettings> _cameras) :
                             fieldLength(_fieldLength),
                             fieldWidth(_fieldWidth),
                             boundaryWidth(_boundaryWidth),
                             lineWidth(_lineWidth),
                             ceilingHeight(_ceilingHeight),
                             goalWidth(_goalWidth),
                             goalWallThickness(_goalWallThickness),
                             goalDepth(_goalDepth),
                             goalHeight(_goalHeight),
                             ballRadius(_ballRadius),
                             ballMass(_ballMass),
                             gravityX(_gravityX),
                             gravityY(_gravityY),
                             gravityZ(_gravityZ),
                             centerCircleRadius(_centerCircleRadius),
                             scale(_scale),
                             cameras(std::move(_cameras)){
}

CameraSettings::CameraSettings(int camID, double focalLength, double principalPointX, double principalPointY,
                               double distortion, double q0, double q1, double q2, double q3, double tx, double ty,
                               double tz, double derivedTX, double derivedTY, double derivedTZ, int xResolution,
                               int yResolution) :
cameraID(camID),
focalLength(focalLength),
principalPointX(principalPointX),
principalPointY(principalPointY),
distortion(distortion),
q0(q0),
q1(q1),
q2(q2),
q3(q3),
tx(tx),
ty(ty),
tz(tz),
derivedTX(derivedTX),
derivedTY(derivedTY),
derivedTZ(derivedTZ),
camResolutionX(xResolution),
camResolutionY(yResolution){

}
