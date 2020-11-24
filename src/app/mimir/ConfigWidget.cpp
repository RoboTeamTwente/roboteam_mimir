//
// Created by rolf on 29-09-19.
//

#include "ConfigWidget.h"
#include "WorldConfig.h"
#include "RobotConfig.h"
#include <QCoreApplication>
#include <iostream>
#include "Situation.h"

ConfigWidget::ConfigWidget() {
    QDir worldDir = findConfigDir();
    QDir robotDir = worldDir;
    QDir situationDir = worldDir;
    if (worldDir.cd("world")) {
        readWorldConfigs(worldDir);
    } else {
        std::cerr << "Could not find world subdirectory!" << std::endl;
    }
    if (robotDir.cd("robot")) {
        readRobotConfigs(robotDir);
    } else {
        std::cerr << "Could not find robot subdirectory!" << std::endl;
    }
    if (situationDir.cd("situations")) {
        readSituations(situationDir);
    } else {
        std::cerr << "Could not find the situations subdirectory!" << std::endl;
    }
}

QDir ConfigWidget::findConfigDir() {
    //ugly but we need to find a way to find the directory
    QString exePath = QCoreApplication::applicationDirPath();
    QDir currentDir(exePath);
    bool success = true;
    while (currentDir.dirName() != "roboteam_mimir") {
        if (currentDir.isRoot()) {
            success = false;
            break;
        }
        currentDir.cdUp();
    }
    if (success) {
        if (currentDir.cd("config")) {
            return currentDir;
        }
    }
    std::cerr << "Could not find the config directory!" << std::endl;
    return currentDir;
}

void ConfigWidget::readRobotConfigs(const QDir &robotDir) {
    QList<QString> files = robotDir.entryList(QDir::Filter::Files);
    for (const auto &fileName : files) {
        QString path = robotDir.absolutePath() + "/" + fileName;
        robotConfigList.push_back(std::move(std::make_unique<RobotConfig>(path)));
    }
}
void ConfigWidget::readWorldConfigs(const QDir &worldDir) {
    //Read every file in the folder and try to turn it into a WorldConfig
    QList<QString> files = worldDir.entryList(QDir::Filter::Files);
    for (const auto &fileName : files) {
        QString path = worldDir.absolutePath() + "/" + fileName;
        worldConfigList.push_back(std::move(std::make_unique<WorldConfig>(path)));
    }
}
void ConfigWidget::readSituations(const QDir &situationDir) {
    QList<QString> files = situationDir.entryList(QDir::Filter::Files);
    for (const auto &fileName : files) {
        QString path = situationDir.absolutePath() + "/" + fileName;
        situationList.push_back(std::move(std::make_unique<Situation>(path)));
    }
}

QList<QString> ConfigWidget::getWorldNames() {
    QList<QString> names;
    for (const auto &config:worldConfigList) {
        names.push_back(config->name());
    }
    return names;
}
QList<QString> ConfigWidget::getRobotNames() {
    QList<QString> names;
    for (const auto &config:robotConfigList) {
        names.push_back(config->name());
    }
    return names;
}
QList<QString> ConfigWidget::getSituationNames() {
    QList<QString> names;
    for (const auto &config:situationList) {
        names.push_back(config->name());
    }
    return names;
}
const std::unique_ptr<WorldConfig> &ConfigWidget::getWorldConfig(const QString &name) {
    for (const auto &config : worldConfigList) {
        if (config->name() == name) {
            return config;
        }
    }
    std::cerr << "Could not find world config: " << name.toStdString() << std::endl;
    return worldConfigList[0];
}
const std::unique_ptr<RobotConfig>& ConfigWidget::getRobotConfig(const QString &name) {
    for (const auto &config : robotConfigList) {
        if (config->name() == name) {
            return config;
        }
    }
    std::cerr << "Could not find robot config: " << name.toStdString() << std::endl;
    return robotConfigList[0];
}
const std::unique_ptr<Situation>& ConfigWidget::getSituation(const QString &name) {
    for (const auto &config : situationList) {
        if (config->name() == name) {
            return config;
        }
    }
    std::cerr << "Could not find Situation" << name.toStdString() << std::endl;
    return situationList[0];
}