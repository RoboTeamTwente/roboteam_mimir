//
// Created by rolf on 07-12-19.
//

#ifndef ROBOTEAM_MIMIR_SITUATIONWORLD_H
#define ROBOTEAM_MIMIR_SITUATIONWORLD_H


#include <string>
#include <vector>
#include <optional>
#include <QtCore/QString>
#include "SituationBall.h"
#include "SituationBot.h"

class SituationWorld {
public:
    SituationWorld(QString _yellowSettings,QString _blueSettings, QString _worldSettings,
            std::vector<SituationBot> blue, std::vector<SituationBot> yellow, std::optional<SituationBall> ball);
    QString yellowSettings;
    QString blueSettings;
    QString worldSettings;
    std::vector<SituationBot> blueBots;
    std::vector<SituationBot> yellowBots;
    std::optional<SituationBall> ball;

};


#endif //ROBOTEAM_MIMIR_SITUATIONWORLD_H
