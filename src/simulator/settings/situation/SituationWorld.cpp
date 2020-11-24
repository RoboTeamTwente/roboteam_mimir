//
// Created by rolf on 07-12-19.
//

#include "settings/situation/SituationWorld.h"

#include <utility>

SituationWorld::SituationWorld(std::vector<SituationBot> blue, std::vector<SituationBot> yellow,
                               std::optional<SituationBall> ball) :
                               blueBots{std::move(blue)},
                               yellowBots{std::move(yellow)},
                               ball{std::move(ball)}{}