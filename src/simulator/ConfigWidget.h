//
// Created by rolf on 29-09-19.
//

#ifndef ROBOTEAM_MIMIR_CONFIGWIDGET_H
#define ROBOTEAM_MIMIR_CONFIGWIDGET_H

#include <QString>
#include <QDir>
#include <QSettings>
#include <memory>

class WorldConfig;
class RobotConfig;
class Situation;
class ConfigWidget {
public:
    ConfigWidget();

    QList<QString> getWorldNames();
    QList<QString> getRobotNames();
    QList<QString> getSituationNames();

    void setCurrentSituation(const QString &name);
    void setCurrentWorld(const QString &name);
    void setCurrentRobot(const QString &name,bool isYellow);

    std::shared_ptr<WorldConfig> getCurrentWorldConfig();
    std::shared_ptr<RobotConfig> getRobotConfig(bool isYellow);
    Situation getSituation();
private:
    static QDir findConfigDir();
    void readWorldConfigs(const QDir &worldDir);
    void readRobotConfigs(const QDir &robotDir);
    void readSituations(const QDir &situationDir);

    QList<Situation *> situationList;
    QList<std::shared_ptr<RobotConfig>> robotConfigList;
    QList<std::shared_ptr<WorldConfig>> worldConfigList;
    std::shared_ptr<WorldConfig> currentWorld = nullptr;
    std::shared_ptr<RobotConfig> blueRobot = nullptr;
    std::shared_ptr<RobotConfig> yellowRobot = nullptr;
    Situation* currentSituation = nullptr; //TODO: fix pointer hell
};


#endif //ROBOTEAM_MIMIR_CONFIGWIDGET_H
