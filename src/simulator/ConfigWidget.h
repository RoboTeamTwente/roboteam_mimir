//
// Created by rolf on 29-09-19.
//

#ifndef ROBOTEAM_MIMIR_CONFIGWIDGET_H
#define ROBOTEAM_MIMIR_CONFIGWIDGET_H

#include <QString>
#include <QDir>
#include <QSettings>
class ConfigWidget {
public:
    ConfigWidget();
    ~ConfigWidget();
private:
    static QDir findConfigDir();
    void readWorldConfigs(const QDir& worldDir);
    void readWorldConfig(const QString& path);
    void readRobotConfigs(const QDir& robotDir);
    void readRobotConfig(const QString& path);

    QList<QSettings*> robotSettings;
    QList<QSettings*> worldSettings;

};


#endif //ROBOTEAM_MIMIR_CONFIGWIDGET_H
