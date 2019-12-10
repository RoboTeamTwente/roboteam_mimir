//
// Created by rolf on 29-09-19.
//

#ifndef ROBOTEAM_MIMIR_CONFIGWIDGET_H
#define ROBOTEAM_MIMIR_CONFIGWIDGET_H

#include <QString>
#include <QDir>
#include <QSettings>
#include <memory>
#include "config/WorldConfig.h"
#include "config/RobotConfig.h"
#include "config/Situation.h"

class ConfigWidget {
public:
    ConfigWidget();

    QList<QString> getWorldNames();
    QList<QString> getRobotNames();
    QList<QString> getSituationNames();

    const std::unique_ptr<WorldConfig>& getWorldConfig(const QString &name);
    const std::unique_ptr<RobotConfig>& getRobotConfig(const QString &name);
    const std::unique_ptr<Situation>& getSituation(const QString &name);
private:
    static QDir findConfigDir();
    void readWorldConfigs(const QDir &worldDir);
    void readRobotConfigs(const QDir &robotDir);
    void readSituations(const QDir &situationDir);

    std::vector<std::unique_ptr<Situation>> situationList;
    std::vector<std::unique_ptr<RobotConfig>> robotConfigList;
    std::vector<std::unique_ptr<WorldConfig>> worldConfigList;

};


#endif //ROBOTEAM_MIMIR_CONFIGWIDGET_H
