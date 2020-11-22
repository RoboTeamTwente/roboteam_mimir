//
// Created by rolf on 07-12-19.
//

#include "SituationWorld.h"

#include <utility>

SituationWorld::SituationWorld( QString  _blueSettings, QString _yellowSettings, QString _worldSettings,
                               std::vector<SituationBot> blue, std::vector<SituationBot> yellow,
                               std::optional<SituationBall> ball) :
                               yellowSettings{std::move(_yellowSettings)},
                               blueSettings{std::move(_blueSettings)},
                               worldSettings{std::move(_worldSettings)},
                               blueBots{std::move(blue)},
                               yellowBots{std::move(yellow)},
                               ball{std::move(ball)}{}