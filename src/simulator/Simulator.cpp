//
// Created by rolf on 27-09-19.
//

#include "Simulator.h"
#include "physics/SimWorld.h"
#include "ConfigWidget.h"
#include "config/WorldConfig.h"
#include "config/RobotConfig.h"
#include "net/Publisher.h"
#include "net/Receiver.h"
#include <QTimer>

Simulator::Simulator() {
    //set up network connections
    QHostAddress localIP("224.5.23.2");
    int sendPort=10006;
    int receiveBluePort=10004;
    publisher=new net::Publisher(localIP,sendPort);
    receiver=new net::Receiver(localIP,receiveBluePort);
    //read all config files and save them in a widget
    configWidget=new ConfigWidget();
    // get the initial config settings and create a physics simulator with them
    WorldSettings* worldSettings= configWidget->getCurrentWorldConfig()->settings;
    RobotSettings* blueSettings = configWidget->getRobotConfig(false)->settings;
    RobotSettings* yellowSettings=configWidget->getRobotConfig(true)->settings;
    simWorld=new SimWorld(worldSettings,blueSettings,yellowSettings);

    //start simulator logic loop
    timer= new QTimer();
    timer->setTimerType(Qt::PreciseTimer);
    connect(timer,&QTimer::timeout,this,&Simulator::tick);
    timer->start(16);

}
Simulator::~Simulator() {
    delete simWorld;
    delete configWidget;
}

//needed for 3d visualization
btDiscreteDynamicsWorld* Simulator::getPhysicsWorld() {
    return simWorld->getWorld();
}

void Simulator::tick() {
    simWorld->stepSimulation();
    SSL_GeometryData data=simWorld->getGeometryData();
    publisher->send(data);
}