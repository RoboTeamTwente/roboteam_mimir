//
// Created by rolf on 07-12-19.
//

#ifndef ROBOTEAM_MIMIR_SITUATIONWORLD_H
#define ROBOTEAM_MIMIR_SITUATIONWORLD_H


#include <string>
#include <vector>
#include <optional>
#include "SituationBall.h"
#include "SituationBot.h"

class SituationWorld {
public:
    SituationWorld(std::vector<SituationBot> blue, std::vector<SituationBot> yellow, std::optional<SituationBall> ball);
    std::vector<SituationBot> blueBots;
    std::vector<SituationBot> yellowBots;
    std::optional<SituationBall> ball;

};


#endif //ROBOTEAM_MIMIR_SITUATIONWORLD_H
