//
// Created by rolf on 27-09-19.
//

#ifndef ROBOTEAM_MIMIR_SIMULATOR_H
#define ROBOTEAM_MIMIR_SIMULATOR_H

#include <QObject>
#include <memory>
#include "net/Publisher.h"
#include "net/Receiver.h"
#include "Timer.h"//TODO: Fix include

#include <graphics/VisualizedSimWorld.h>

#include "ConfigWidget.h"
#include "WorldConfig.h"
#include "RobotConfig.h"

#include <QTimer>
#include "proto/messages_robocup_ssl_geometry.pb.h"
#include "Timer.h"

class Simulator :public QObject {
    Q_OBJECT
public:
    Simulator();
    btDiscreteDynamicsWorld* getPhysicsWorld();
    VisualizedSimWorld * getVisualWorld();
    WorldSettings getWorldSettings();
    RobotSettings getBlueSettings();
    RobotSettings getYellowSettings();
    QList<QString> getWorldConfigNames();
    QList<QString> getRobotConfigNames();
    SSL_GeometryData getGeometry();

signals:
    void geometryUpdated(const SSL_GeometryData &geometry,WorldSettings settings);
    void blueUpdated(RobotSettings settings);
    void yellowUpdated(RobotSettings settings);
    void sentPackets(const std::vector<SSL_WrapperPacket> &packets);
public slots:
    void tick();

    void setVisionIP(const QString &address);
    void setVisionPort(int port);
    void setBlueIP(const QString &address);
    void setBluePort(int port);
    void setYellowIP(const QString &address);
    void setYellowPort(int port);

    void setWorldConfig(const QString &name);
    void setYellowConfig(const QString &name);
    void setBlueConfig(const QString &name);

    void setBlueBotCount(int count);
    void setYellowBotCount(int count);

    void setBallXNoise(double noise);
    void setBallYNoise(double noise);
    void setBallVanishing(double vanishingProb);

    void setRobotXNoise(double noise);
    void setRobotYNoise(double noise);
    void setRobotOrNoise(double noise);
    void setRobotVanishing(double vanishingProb);

    void setDelay(double delay);
    void setSendGeometryTicks(int ticks);
private:
    void setRobotConfig(const QString &name, bool isYellow);
    QTimer *timer;
    std::unique_ptr<Timer> timingManager;
    std::unique_ptr<Timing> currentTiming;
    std::unique_ptr<VisualizedSimWorld> simWorld;
    ConfigWidget* configWidget; //TODO: how to make this a unique pointer?
    std::unique_ptr<net::Publisher> publisher;
    std::unique_ptr<net::Receiver> blueReceiver;
    std::unique_ptr<net::Receiver> yellowReceiver;

};


#endif //ROBOTEAM_MIMIR_SIMULATOR_H
