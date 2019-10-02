//
// Created by rolf on 29-09-19.
//

#include "ConfigWidget.h"
#include "config/WorldConfig.h"
#include "config/RobotConfig.h"
#include <QCoreApplication>
#include <iostream>
ConfigWidget::ConfigWidget() {
    QDir worldDir=findConfigDir();
    QDir robotDir=worldDir;
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
}
ConfigWidget::~ConfigWidget() {
    while(!worldConfigList.empty()){
        delete worldConfigList.first();
    }
    delete currentWorld;
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
        RobotConfig * robotConfig=new RobotConfig(path);
        robotConfigList.push_back(robotConfig);
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
        WorldConfig *world=new WorldConfig(path);
        worldConfigList.push_back(world);
        if(!currentWorld){
            currentWorld=world;
        }
    }
}
void ConfigWidget::setCurrentWorld(QString &name) {
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
WorldConfig* ConfigWidget::getCurrentWorldConfig() {
    return currentWorld;
}
RobotConfig * ConfigWidget::getRobotConfig(bool isYellow) {
    return isYellow?  yellowRobot : blueRobot;
}