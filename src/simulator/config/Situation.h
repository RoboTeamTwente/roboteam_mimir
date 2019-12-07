//
// Created by rolf on 07-12-19.
//

#ifndef ROBOTEAM_MIMIR_SITUATION_H
#define ROBOTEAM_MIMIR_SITUATION_H

#include <QtCore/QSettings>
#include "SituationWorld.h"
#include <memory>

class Situation {
public:
    explicit Situation(const QString &filepath);
    QString name() const;

    std::unique_ptr<SituationWorld> situation;
private:
    std::unique_ptr<QSettings> situationFile;
};


#endif //ROBOTEAM_MIMIR_SITUATION_H
