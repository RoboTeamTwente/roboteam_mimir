//
// Created by rolf on 29-09-19.
//

#ifndef ROBOTEAM_MIMIR_WORLDCONFIG_H
#define ROBOTEAM_MIMIR_WORLDCONFIG_H

#include <QSettings>
#include "WorldSettings.h"
#include <memory>


class WorldConfig {
public:
    explicit WorldConfig(const QString& filepath);
    void reloadSettings();
    QString name() const;
    std::shared_ptr<WorldSettings> settings;
private:
    float get(const QString& valueString) const;

    std::unique_ptr<QSettings> settingsFile;
};



#endif //ROBOTEAM_MIMIR_WORLDCONFIG_H
