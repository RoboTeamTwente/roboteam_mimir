//
// Created by rolf on 29-09-19.
//

#ifndef ROBOTEAM_MIMIR_ROBOTCONFIG_H
#define ROBOTEAM_MIMIR_ROBOTCONFIG_H

#include <QSettings>
#include "RobotSettings.h"
#include <memory>

class RobotConfig {
public:
    explicit RobotConfig(const QString &filepath);
    QString name() const;
    std::unique_ptr<RobotSettings> settings;
private:
    float get(const QString &valueString) const; //only used to load the const variables into the settings format
    std::unique_ptr<QSettings> settingsFile;

};


#endif //ROBOTEAM_MIMIR_ROBOTCONFIG_H
