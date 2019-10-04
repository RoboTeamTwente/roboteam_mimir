//
// Created by rolf on 29-09-19.
//

#ifndef ROBOTEAM_MIMIR_CONFIGWIDGET_H
#define ROBOTEAM_MIMIR_CONFIGWIDGET_H

#include <QString>
#include <QDir>
#include <QSettings>

class WorldConfig;
class RobotConfig;

class ConfigWidget {
public:
    ConfigWidget();
    ~ConfigWidget();

    QList<QString> getWorldNames();
    void setCurrentWorld(QString &name);
    WorldConfig *getCurrentWorldConfig();
    RobotConfig *getRobotConfig(bool isYellow);
private:
    static QDir findConfigDir();
    void readWorldConfigs(const QDir &worldDir);
    void readRobotConfigs(const QDir &robotDir);
    QList<RobotConfig *> robotConfigList;
    QList<WorldConfig *> worldConfigList;
    WorldConfig *currentWorld = nullptr;
    RobotConfig *blueRobot = nullptr;
    RobotConfig *yellowRobot = nullptr;
};


#endif //ROBOTEAM_MIMIR_CONFIGWIDGET_H
