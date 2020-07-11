//
// Created by rolf on 02-10-19.
//

#include "WorldSettings.h"

#include <utility>

WorldSettings::WorldSettings(float _fieldLength, float _fieldWidth, float _boundaryWidth, float _lineWidth,
                             float _ceilingHeight, float _goalWidth, float _goalWallThickness, float _goalDepth,
                             float _goalHeight, float _ballRadius, float _ballMass, float _gravityX, float _gravityY,
                             float _gravityZ, float _centerCircleRadius, float _scale, std::vector<CameraSettings> _cameras) :
                             _fieldLength(_fieldLength),
                             _fieldWidth(_fieldWidth),
                             _boundaryWidth(_boundaryWidth),
                             _lineWidth(_lineWidth),
                             _ceilingHeight(_ceilingHeight),
                             _goalWidth(_goalWidth),
                             _goalWallThickness(_goalWallThickness),
                             _goalDepth(_goalDepth),
                             _goalHeight(_goalHeight),
                             _ballRadius(_ballRadius),
                             _ballMass(_ballMass),
                             _gravityX(_gravityX),
                             _gravityY(_gravityY),
                             _gravityZ(_gravityZ),
                             _centerCircleRadius(_centerCircleRadius),
                             _scale(_scale),
                             _cameras(std::move(_cameras)){
}
float WorldSettings::fieldLength() const{
    return _fieldLength;
}
float WorldSettings::fieldWidth() const{
    return _fieldWidth;
}
float WorldSettings::boundaryWidth() const{
    return _boundaryWidth;
}
float WorldSettings::lineWidth() const{
    return _lineWidth;
}
float WorldSettings::ceilingHeight() const{
    return _ceilingHeight;
}
float WorldSettings::goalWidth() const{
    return _goalWidth;
}
float WorldSettings::goalWallThickness() const{
    return _goalWallThickness;
}
float WorldSettings::goalDepth() const{
    return _goalDepth;
}
float WorldSettings::goalHeight() const{
    return _goalHeight;
}
float WorldSettings::ballRadius() const{
    return _ballRadius;
}
float WorldSettings::ballMass() const{
    return _ballMass;
}
float WorldSettings::gravityX() const{
    return _gravityX;
}
float WorldSettings::gravityY() const{
    return _gravityY;
}
float WorldSettings::gravityZ() const{
    return _gravityZ;
}
float WorldSettings::centerCircleRadius() const{
    return _centerCircleRadius;
}
float WorldSettings::scale() const{
    return _scale;
}
const std::vector<CameraSettings> &WorldSettings::cameras() const{
    return _cameras;
}

CameraSettings::CameraSettings(int camID, double focalLength, double principalPointX, double principalPointY,
                               double distortion, double q0, double q1, double q2, double q3, double tx, double ty,
                               double tz, double derivedTX, double derivedTY, double derivedTZ, int xResolution,
                               int yResolution) :
_cameraID(camID),
_focalLength(focalLength),
_principalPointX(principalPointX),
_principalPointY(principalPointY),
_distortion(distortion),
_q0(q0),
_q1(q1),
_q2(q2),
_q3(q3),
_tx(tx),
_ty(ty),
_tz(tz),
_derivedTX(derivedTX),
_derivedTY(derivedTY),
_derivedTZ(derivedTZ),
_camResolutionX(xResolution),
_camResolutionY(yResolution){

}
int CameraSettings::cameraID() const {
    return _cameraID;
}
double CameraSettings::focalLength() const {
    return _focalLength;
}
double CameraSettings::principalPointX() const {
    return _principalPointX;
}
double CameraSettings::principalPointY() const {
    return _principalPointY;
}
double CameraSettings::distortion() const {
    return _distortion;
}
double CameraSettings::q0() const {
    return _q0;
}
double CameraSettings::q1() const {
    return _q1;
}
double CameraSettings::q2() const {
    return _q2;
}
double CameraSettings::q3() const {
    return _q3;
}
double CameraSettings::tx() const {
    return _tx;
}
double CameraSettings::ty() const {
    return _ty;
}
double CameraSettings::tz() const {
    return _tz;
}
double CameraSettings::derivedTX() const {
    return _derivedTX;
}
double CameraSettings::derivedTY() const {
    return _derivedTY;
}
double CameraSettings::derivedTZ() const {
    return _derivedTZ;
}
int CameraSettings::camResolutionX() const {
    return _camResolutionX;
}
int CameraSettings::camResolutionY() const {
    return _camResolutionY;
}
