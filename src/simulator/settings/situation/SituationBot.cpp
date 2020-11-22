//
// Created by rolf on 07-12-19.
//

#include "SituationBot.h"

SituationBot::SituationBot(unsigned int _id, double x, double y, double ang, double velX, double velY, double velAng) :
    id{_id},
    position{btVector3(x,y,ang)},
    velocity{btVector3(velX,velY,velAng)}
    {
}