//
// Created by rolf on 02-10-19.
//

#include "WorldSettings.h"

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

CameraSettings::CameraSettings(std::string camInfo, int xResolution, int yResolution) :
camInfoSerialized{std::move(camInfo)},
camResolutionX(xResolution),
camResolutionY{yResolution}{

}