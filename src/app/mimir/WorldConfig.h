//
// Created by rolf on 29-09-19.
//

#ifndef ROBOTEAM_MIMIR_WORLDCONFIG_H
#define ROBOTEAM_MIMIR_WORLDCONFIG_H

#include <QSettings>
#include <simulator/settings/WorldSettings.h>
#include <memory>


class WorldConfig {
public:
    explicit WorldConfig(const QString& filepath);
    QString name() const;
    std::unique_ptr<WorldSettings> settings;
private:
    float get(const QString& valueString) const;

    std::unique_ptr<QSettings> settingsFile;
};



#endif //ROBOTEAM_MIMIR_WORLDCONFIG_H
