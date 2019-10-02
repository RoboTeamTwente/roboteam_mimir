//
// Created by rolf on 29-09-19.
//

#ifndef ROBOTEAM_MIMIR_ROBOTCONFIG_H
#define ROBOTEAM_MIMIR_ROBOTCONFIG_H

#include <QSettings>
#include "RobotSettings.h"


class RobotConfig {
public:
    explicit RobotConfig(const QString &filepath);
    ~RobotConfig();
    void reloadSettings();
    QString name() const;
    RobotSettings* settings;
private:
    float get(const QString &valueString) const; //only used to load the const variables into the settings format
    QSettings *settingsFile;

};


#endif //ROBOTEAM_MIMIR_ROBOTCONFIG_H
