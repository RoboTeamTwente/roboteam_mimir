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
    SituationWorld(std::string _yellowSettings,std::string _blueSettings, std::string _worldSettings,
            std::vector<SituationBot> blue, std::vector<SituationBot> yellow, std::optional<SituationBall> ball);
private:
    const std::string yellowSettings;
    const std::string blueSettings;
    const std::string worldSettings;
    const std::vector<SituationBot> blueBots;
    const std::vector<SituationBot> yellowBots;
    const std::optional<SituationBall> ball;

};


#endif //ROBOTEAM_MIMIR_SITUATIONWORLD_H
