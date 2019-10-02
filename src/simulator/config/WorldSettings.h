//
// Created by rolf on 02-10-19.
//

#ifndef ROBOTEAM_MIMIR_WORLDSETTINGS_H
#define ROBOTEAM_MIMIR_WORLDSETTINGS_H


class WorldSettings {
public:
    WorldSettings(
            float _fieldLength,
            float _fieldWidth,
            float _boundaryWidth,
            float _lineWidth,
            float _ceilingHeight,
            float _goalWidth,
            float _goalWallThickness,
            float _goalDepth,
            float _goalHeight,
            float _ballRadius,
            float _ballMass,
            float _gravityX,
            float _gravityY,
            float _gravityZ
    );
    const float fieldLength;
    const float fieldWidth;
    const float boundaryWidth;
    const float lineWidth;
    const float ceilingHeight;
    const float goalWidth;
    const float goalWallThickness;
    const float goalDepth;
    const float goalHeight;
    const float ballRadius;
    const float ballMass;
    const float gravityX;
    const float gravityY;
    const float gravityZ;
};


#endif //ROBOTEAM_MIMIR_WORLDSETTINGS_H
