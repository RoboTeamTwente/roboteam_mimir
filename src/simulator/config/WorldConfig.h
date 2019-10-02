//
// Created by rolf on 29-09-19.
//

#ifndef ROBOTEAM_MIMIR_WORLDCONFIG_H
#define ROBOTEAM_MIMIR_WORLDCONFIG_H

#include <QSettings>
#include "WorldSettings.h"



class WorldConfig {
public:
    explicit WorldConfig(const QString& filepath);
    ~WorldConfig();
    void reloadSettings();
    QString name() const;
    WorldSettings* settings;
private:
    float get(const QString& valueString) const;

    QSettings* settingsFile;
};



#endif //ROBOTEAM_MIMIR_WORLDCONFIG_H
