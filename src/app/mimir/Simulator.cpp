//
// Created by rolf on 27-09-19.
//

#include "Simulator.h"


Simulator::Simulator() {
    //set up network connections
    //TODO: make these settings saved in sim
    QHostAddress localIP("224.5.23.2");
    int sendPort=10006;
    int receiveBluePort=20011;
    int receiveYellowPort=20011;
    publisher=std::make_unique<net::Publisher>(localIP,sendPort);
    blueReceiver=std::make_unique<net::Receiver>(QHostAddress("127.0.0.1"),receiveBluePort);//TODO: change back to localIP
    yellowReceiver=std::make_unique<net::Receiver>(QHostAddress("127.0.0.1"),receiveYellowPort);
    //read all config files and save them in a widget
    configWidget=new ConfigWidget();
    // get the initial config settings and create a physics simulator with them
    SituationWorld situation = *configWidget->getSituation("BallTestFormation")->situation.get();
    WorldSettings worldSettings = *configWidget->getWorldConfig("divisionA")->settings.get();
    RobotSettings blue_settings= *configWidget->getRobotConfig("RTT")->settings.get();
    RobotSettings yellow_settings= *configWidget->getRobotConfig("RTT")->settings.get();
    simWorld=std::make_unique<VisualizedSimWorld>(worldSettings,blue_settings,yellow_settings,situation);
    simWorld->reloadSituation();
    timingManager=std::make_unique<Timer>();
    currentTiming=std::make_unique<Timing>(timingManager->getTimer("Realtime 250hz"));
    //start simulator logic loop
    timer= new QTimer();
    timer->setTimerType(Qt::PreciseTimer);
    connect(timer,&QTimer::timeout,this,&Simulator::tick);
    timer->start(currentTiming->millisecWaitTime);

}

//needed for 3d visualization
btDiscreteDynamicsWorld* Simulator::getPhysicsWorld() {
    return simWorld->getWorld();
}

void Simulator::tick() {
    static auto sum = std::chrono::nanoseconds(0);
    static int ticks = 0;
    auto start=std::chrono::high_resolution_clock::now();
    //TODO: fix input delay and make receivers callback based on a seperate thread so we can keep the looprate low but the internal step of the world high
    auto blueMsgs=blueReceiver->readMessages();
    auto yellowMsgs=yellowReceiver->readMessages();

    std::vector<SSL_WrapperPacket> packets=simWorld->stepSimulation(blueMsgs,yellowMsgs);
    for (const auto& packet: packets){
        publisher->send(packet);
    }
    emit sentPackets(packets);
    auto end=std::chrono::high_resolution_clock::now();
    sum += (end-start);
    ticks++;
    if(ticks == 250 ) {
      std::cout << "loop took " << std::chrono::duration_cast<std::chrono::microseconds>(sum).count() /250 << " us";
      std::cout << " dt" << currentTiming->timeStepLength << " ms: " << currentTiming->millisecWaitTime << std::endl;
      ticks = 0;
      sum = std::chrono::nanoseconds(0);
    }
}

WorldSettings Simulator::getWorldSettings() {
    return simWorld->getWorldSettings();
}
RobotSettings Simulator::getBlueSettings() {
    return simWorld->getRobotSettings(false);
}
RobotSettings Simulator::getYellowSettings() {
    return simWorld->getRobotSettings(true);
}
QList<QString> Simulator::getRobotConfigNames() {
    return configWidget->getRobotNames();
}
QList<QString> Simulator::getWorldConfigNames() {
    return configWidget->getWorldNames();
}
void Simulator::setRobotConfig(const QString &name, bool isYellow) {
    simWorld->setRobotSettings(*configWidget->getRobotConfig(name)->settings.get(),isYellow);
    if (isYellow){
        emit yellowUpdated(getYellowSettings());
    }
    else{
        emit blueUpdated(getBlueSettings());
    }
}
void Simulator::setBlueConfig(const QString &name) {
    std::cerr<<"setting Blue Config to "<< name.toStdString()<<std::endl;
    setRobotConfig(name,false);
}

void Simulator::setYellowConfig(const QString &name) {
    std::cerr<<"setting Yellow Config to "<< name.toStdString()<<std::endl;
    setRobotConfig(name,true);
}
void Simulator::setWorldConfig(const QString &name) {
    std::cerr<<"setting worldConfig to "<< name.toStdString()<<std::endl;
    simWorld->setWorldSettings(*configWidget->getWorldConfig(name)->settings.get());
    emit geometryUpdated(simWorld->getGeometryData(),getWorldSettings());
}
void Simulator::setBlueBotCount(int count) {
    if (count<0 || count > 16){
        std::cerr<<"Invalid number of robots"<<std::endl;
    }
    std::cout<<count<<" blue bots"<<std::endl;
    simWorld->setRobotCount(count,false);
}
void Simulator::setYellowBotCount(int count) {
    if (count<0 || count > 16){
        std::cerr<<"Invalid number of robots"<<std::endl;
    }
    std::cout<<count<<" yellow bots"<<std::endl;
    simWorld->setRobotCount(count,true);
}

void Simulator::setVisionIP(const QString &address) {
    publisher->setIP(QHostAddress(address));
}
void Simulator::setVisionPort(int port) {
    if (port< 0 || port>999999){
        std::cerr<<"invalid port"<<std::endl;
        return;
    }
    publisher->setPort(port);
}
void Simulator::setBluePort(int port) {
    if (port< 0 || port>999999){
        std::cerr<<"invalid port"<<std::endl;
        return;
    }
    blueReceiver->setPort(port);
}
void Simulator::setYellowPort(int port) {
    if (port< 0 || port>999999){
        std::cerr<<"invalid port"<<std::endl;
        return;
    }
    yellowReceiver->setPort(port);
}
void Simulator::setBlueIP(const QString &address) {
    blueReceiver->setIP(QHostAddress(address));
}
void Simulator::setYellowIP(const QString &address) {
    yellowReceiver->setIP(QHostAddress(address));
}
void Simulator::setSendGeometryTicks(int ticks) {
    if (ticks<0){
        std::cerr<<"Can't set to a negative number of ticks"<<std::endl;
    }
    simWorld->setSendGeometryTicks(ticks);
}
SSL_GeometryData Simulator::getGeometry() {
    return simWorld->getGeometryData();
}
void Simulator::setBallXNoise(double noise) {
    simWorld->setBallXNoise(noise);
}
void Simulator::setBallYNoise(double noise) {
    simWorld->setBallYNoise(noise);
}
void Simulator::setBallVanishing(double vanishingProb) {
    simWorld->setBallVanishing(vanishingProb);
}
void Simulator::setRobotXNoise(double noise) {
    simWorld->setRobotXNoise(noise);
}
void Simulator::setRobotYNoise(double noise) {
    simWorld->setRobotYNoise(noise);
}
void Simulator::setRobotOrNoise(double noise) {
    simWorld->setRobotOrNoise(noise);
}
void Simulator::setRobotVanishing(double vanishingProb) {
    simWorld->setRobotVanishing(vanishingProb);
}
void Simulator::setDelay(double delay) {
    simWorld->setDelay(delay);
}
VisualizedSimWorld *Simulator::getVisualWorld() {
  return simWorld.get();
}
