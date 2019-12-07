//
// Created by rolf on 29-09-19.
//

#include "ConfigWidget.h"
#include "config/WorldConfig.h"
#include "config/RobotConfig.h"
#include <QCoreApplication>
#include <iostream>
#include "config/Situation.h"

ConfigWidget::ConfigWidget() {
    QDir worldDir=findConfigDir();
    QDir robotDir=worldDir;
    QDir situationDir=worldDir;
    if(worldDir.cd("world")){
        readWorldConfigs(worldDir);
    }
    else{
        std::cerr<<"Could not find world subdirectory!"<<std::endl;
    }
    if(robotDir.cd("robot")){
        readRobotConfigs(robotDir);
    }
    else{
        std::cerr<<"Could not find robot subdirectory!"<<std::endl;
    }
    if (situationDir.cd("situations")){
        readSituations(situationDir);
    }
    else{
        std::cerr<<"Could not find the situations subdirectory!"<<std::endl;
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
        if (currentDir.cd("src/simulator/config")) {
            return currentDir;
        }
    }
    std::cerr << "Could not find the config directory!" << std::endl;
    return currentDir;
}

void ConfigWidget::readRobotConfigs(const QDir& robotDir) {
    QList<QString> files=robotDir.entryList(QDir::Filter::Files);
    for (const auto & fileName : files){
        QString path=robotDir.absolutePath()+"/"+fileName;
        std::shared_ptr<RobotConfig> robotConfig=std::make_shared<RobotConfig>(path);
        robotConfigList.push_back(robotConfig);
        //we put the configs on the first file by default.
        if(!blueRobot){
            blueRobot=robotConfig;
        }
        if(!yellowRobot){
            yellowRobot=robotConfig;
        }
    }
}
void ConfigWidget::readWorldConfigs(const QDir& worldDir) {
    //Read every file in the folder and try to turn it into a WorldConfig
    QList<QString> files=worldDir.entryList(QDir::Filter::Files);
    for (const auto & fileName : files){
        QString path=worldDir.absolutePath()+"/"+fileName;
        std::shared_ptr<WorldConfig> world=std::make_shared<WorldConfig>(path);
        worldConfigList.push_back(world);
        if(!currentWorld){
            currentWorld=world;
        }
    }
}
void ConfigWidget::readSituations(const QDir &situationDir) {
    QList<QString> files=situationDir.entryList(QDir::Filter::Files);
    for (const auto & fileName : files){
        QString path = situationDir.absolutePath() +"/" +fileName;
        Situation * situation = new Situation(path);
        if (!currentSituation){
            currentSituation=situation;
        }
    }
}
void ConfigWidget::setCurrentSituation(const QString &name) {
    for (const auto& config : situationList){
        if (config->name()==name){
            currentSituation=config;
            return;
        }
    }
    std::cerr<<"Could not find the Situation: " << name.toStdString()<<std::endl;
}
void ConfigWidget::setCurrentWorld(const QString &name) {
    for (const auto& config : worldConfigList) {
        if(config->name()==name){
            currentWorld=config;
            return;
        }
    }
    std::cerr<<"Could not find world config: " <<name.toStdString()<<std::endl;
}
QList<QString> ConfigWidget::getWorldNames() {
    QList<QString> names;
    for (const auto& config:worldConfigList) {
        names.push_back(config->name());
    }
    return names;
}
QList<QString> ConfigWidget::getRobotNames() {
    QList<QString> names;
    for (const auto& config:robotConfigList) {
        names.push_back(config->name());
    }
    return names;
}
QList<QString> ConfigWidget::getSituationNames() {
    QList<QString> names;
    for (const auto& config:situationList) {
        names.push_back(config->name());
    }
    return names;
}
std::shared_ptr<WorldConfig> ConfigWidget::getCurrentWorldConfig() {
    return currentWorld;
}
std::shared_ptr<RobotConfig>ConfigWidget::getRobotConfig(bool isYellow) {
    return isYellow?  yellowRobot : blueRobot;
}

void ConfigWidget::setCurrentRobot(const QString &name, bool isYellow) {
    for (const auto& config : robotConfigList) {
        if(config->name()==name){
            if (isYellow){
                yellowRobot = config;
            }
            else{
                blueRobot = config;
            }
            return;
        }
    }
    std::cerr<<"Could not find robot config: " <<name.toStdString()<<std::endl;
}