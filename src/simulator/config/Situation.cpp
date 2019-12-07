//
// Created by rolf on 07-12-19.
//

#include "Situation.h"
#include <QFileInfo>

Situation::Situation(const QString &filepath) {
    situationFile = std::make_unique<QSettings>(filepath, QSettings::IniFormat);

    const QString xPosString("PosX");
    const QString yPosString("PosY");
    const QString angString("Angle");
    const QString xVelString("VelX");
    const QString yVelString("VelY");
    const QString velAngString("VelAng");
    const QString IDString("ID");



    std::vector<SituationBot> yellowBots;
    int yellowSize = situationFile->beginReadArray("Yellow");
    for (int i = 0; i < yellowSize; ++i) {
        situationFile->setArrayIndex(i);
        unsigned int id = situationFile->value(IDString, 0.0).toUInt();
        double x = situationFile->value(xPosString, 0.0).toDouble();
        double y = situationFile->value(yPosString, 0.0).toDouble();
        double ang = situationFile->value(angString, 0.0).toDouble();
        double vx = situationFile->value(xVelString, 0.0).toDouble();
        double vy = situationFile->value(yVelString, 0.0).toDouble();
        double angVel = situationFile->value(velAngString, 0.0).toDouble();
        SituationBot bot(id, x, y, ang, vx, vy, angVel);
        yellowBots.push_back(bot);
    }
    situationFile->endArray();


    std::vector<SituationBot> blueBots;
    int blueSize = situationFile->beginReadArray("Blue");
    for (int i = 0; i < blueSize; ++i) {
        situationFile->setArrayIndex(i);
        unsigned int id = situationFile->value(IDString, 0.0).toUInt();
        double x = situationFile->value(xPosString, 0.0).toDouble();
        double y = situationFile->value(yPosString, 0.0).toDouble();
        double ang = situationFile->value(angString, 0.0).toDouble();
        double vx = situationFile->value(xVelString, 0.0).toDouble();
        double vy = situationFile->value(yVelString, 0.0).toDouble();
        double angVel = situationFile->value(velAngString, 0.0).toDouble();
        SituationBot bot(id, x, y, ang, vx, vy, angVel);
        blueBots.push_back(bot);
    }
    situationFile->endArray();

    std::optional<SituationBall> ball = std::nullopt;
    const QString ballString("Ball");
    if (situationFile->contains(ballString + "/" + xPosString)) {
        double x = situationFile->value(ballString + "/" + xPosString, 0.0).toDouble();
        double y = situationFile->value(ballString + "/" + yPosString, 0.0).toDouble();
        double vx = situationFile->value(ballString + "/" + xVelString, 0.0).toDouble();
        double vy = situationFile->value(ballString + "/" + yVelString, 0.0).toDouble();
        ball.emplace(SituationBall(x, y, vx, vy));
    }
    std::string yellowSettings = situationFile->value("YellowSettings", "").toString().toStdString();
    std::string blueSettings = situationFile->value("BlueSettings", "").toString().toStdString();
    std::string worldSettings = situationFile->value("WorldSettings", "").toString().toStdString();
    situation = std::make_unique<SituationWorld>(yellowSettings, blueSettings, worldSettings, blueBots, yellowBots,
                                                 ball);
}

QString Situation::name() const {
    if (situationFile) {
        return QFileInfo(situationFile->fileName()).baseName(); //return name without path or extensions
    }
    return "";
}