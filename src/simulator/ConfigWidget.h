//
// Created by rolf on 29-09-19.
//

#ifndef ROBOTEAM_MIMIR_CONFIGWIDGET_H
#define ROBOTEAM_MIMIR_CONFIGWIDGET_H

#include <QString>
#include <QDir>
#include <QSettings>
class WorldConfig;
class ConfigWidget {
public:
    ConfigWidget();
    ~ConfigWidget();
    WorldConfig const* getCurrentWorldConfig();

private:
    static QDir findConfigDir();
    void readWorldConfigs(const QDir& worldDir);
    void readRobotConfigs(const QDir& robotDir);

    //QList<QSettings*> robotSettings;
    QList<WorldConfig*> worldConfigList;
    WorldConfig* currentWorld=nullptr;
};


#endif //ROBOTEAM_MIMIR_CONFIGWIDGET_H
