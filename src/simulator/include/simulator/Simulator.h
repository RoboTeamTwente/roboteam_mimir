//
// Created by rolf on 27-09-19.
//

#ifndef ROBOTEAM_MIMIR_SIMULATOR_H
#define ROBOTEAM_MIMIR_SIMULATOR_H

#include <QObject>
#include <memory>

class SimWorld;
class ConfigWidget;
class btDiscreteDynamicsWorld;
class WorldSettings;
class RobotSettings;
class SSL_GeometryData;
class SSL_WrapperPacket;
namespace net{
    class Publisher;
    class Receiver;
}
class QTimer;
class Simulator :public QObject {
    Q_OBJECT
public:
    Simulator();
    ~Simulator();
    btDiscreteDynamicsWorld* getPhysicsWorld();
    WorldSettings * getWorldSettings();
    RobotSettings * getBlueSettings();
    RobotSettings * getYellowSettings();
    QList<QString> getWorldConfigNames();
    QList<QString> getRobotConfigNames();
    SSL_GeometryData getGeometry();

signals:
    void geometryUpdated(const SSL_GeometryData &geometry,WorldSettings* settings);
    void blueUpdated(RobotSettings* settings);
    void yellowUpdated(RobotSettings* settings);
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

    void setSendGeometryTicks(int ticks);
private:
    void setRobotConfig(const QString &name, bool isYellow);
    QTimer *timer;
    SimWorld* simWorld;
    ConfigWidget* configWidget;
    std::unique_ptr<net::Publisher> publisher;
    std::unique_ptr<net::Receiver> blueReceiver;
    std::unique_ptr<net::Receiver> yellowReceiver;

};


#endif //ROBOTEAM_MIMIR_SIMULATOR_H
