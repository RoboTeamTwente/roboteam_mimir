//
// Created by rolf on 07-12-19.
//

#ifndef ROBOTEAM_MIMIR_SITUATION_H
#define ROBOTEAM_MIMIR_SITUATION_H


#include <string>
#include <vector>
#include <optional>
#include "SituationBall.h"
#include "SituationBot.h"

class Situation {
private:
    std::string yellowSettings;
    std::string blueSettings;
    std::string worldSettings;
    std::vector<SituationBot> blueBots;
    std::vector<SituationBot> yellowBots;
    std::optional<SituationBall> ball;

};


#endif //ROBOTEAM_MIMIR_SITUATION_H
