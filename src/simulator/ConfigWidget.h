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

class ConfigWidget {
public:
    ConfigWidget();

    QList<QString> getWorldNames();
    QList<QString> getRobotNames();
    void setCurrentWorld(const QString &name);
    void setCurrentRobot(const QString &name,bool isYellow);
    std::shared_ptr<WorldConfig> getCurrentWorldConfig();
    std::shared_ptr<RobotConfig> getRobotConfig(bool isYellow);
private:
    static QDir findConfigDir();
    void readWorldConfigs(const QDir &worldDir);
    void readRobotConfigs(const QDir &robotDir);
    QList<std::shared_ptr<RobotConfig>> robotConfigList;
    QList<std::shared_ptr<WorldConfig>> worldConfigList;
    std::shared_ptr<WorldConfig> currentWorld = nullptr;
    std::shared_ptr<RobotConfig> blueRobot = nullptr;
    std::shared_ptr<RobotConfig> yellowRobot = nullptr;
};


#endif //ROBOTEAM_MIMIR_CONFIGWIDGET_H
