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
    //TODO: make these settings saved in sim and easy to adjust
    QHostAddress localIP("127.0.0.1");
    int sendPort=10006;
    int receiveBluePort=10004;
    int receiveYellowPort=10003;
    publisher=new net::Publisher(localIP,sendPort);
    blueReceiver=new net::Receiver(localIP,receiveBluePort);
    blueReceiver=new net::Receiver(localIP,receiveYellowPort);
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
    //TODO: fix input delay and make receivers callback based on a seperate thread so we can keep the looprate low but the internal step of the world high
    auto blueMsgs=blueReceiver->readMessages();
    auto yellowMsgs=yellowReceiver->readMessages();
    simWorld->stepSimulation();
    std::vector<SSL_WrapperPacket> packets=simWorld->getPackets();
    for (const auto& packet: packets){
        publisher->send(packet);
    }
}

WorldSettings* Simulator::getWorldSettings() {
    return configWidget->getCurrentWorldConfig()->settings;
}