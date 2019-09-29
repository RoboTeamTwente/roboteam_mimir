//
// Created by rolf on 29-09-19.
//

#include "ConfigWidget.h"
#include "WorldConfig.h"
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
    }
}
void ConfigWidget::readWorldConfigs(const QDir& worldDir) {
    //Read every file in the folder and try to turn it into a WorldConfig
    QList<QString> files=worldDir.entryList(QDir::Filter::Files);
    for (const auto & fileName : files){
        QString path=worldDir.absolutePath()+"/"+fileName;
        WorldConfig *world=new WorldConfig(path);
        worldConfigList.push_back(world);
        if (!currentWorld){
            currentWorld=world;
        }
    }
}
WorldConfig const* ConfigWidget::getCurrentWorldConfig() {
    return currentWorld;
}