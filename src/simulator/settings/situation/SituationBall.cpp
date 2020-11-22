//
// Created by rolf on 07-12-19.
//

#include "SituationBall.h"
SituationBall::SituationBall(double x, double y, double xVel, double yVel) :
isRolling{true},
position{btVector3(x,y,0.0)},
velocity{btVector3(xVel,yVel,0.0)}
{
}