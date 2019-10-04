//
// Created by rolf on 27-09-19.
//

#include "Simulator.h"
#include "physics/SimWorld.h"
#include "ConfigWidget.h"
#include "config/WorldConfig.h"
#include "config/RobotConfig.h"
Simulator::Simulator() {
    //read all config files and save them in a widget
    configWidget=new ConfigWidget();
    // get the initial config settings and create a physics simulator with them
    WorldSettings* worldSettings= configWidget->getCurrentWorldConfig()->settings;
    RobotSettings* blueSettings = configWidget->getRobotConfig(false)->settings;
    RobotSettings* yellowSettings=configWidget->getRobotConfig(true)->settings;
    simWorld=new SimWorld(worldSettings,blueSettings,yellowSettings);

}
Simulator::~Simulator() {
    delete simWorld;
    delete configWidget;
}

btDiscreteDynamicsWorld* Simulator::getPhysicsWorld() {
    return simWorld->getWorld();
}