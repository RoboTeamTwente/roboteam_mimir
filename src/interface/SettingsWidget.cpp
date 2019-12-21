//
// Created by rolf on 27-11-19.
//

#include "SettingsWidget.h"
#include "simulator/Simulator.h"
namespace interface{
    SettingsWidget::SettingsWidget(Simulator* simulator,QWidget *parent) : QWidget(parent){
        //TODO: connect widgets and initialize values correctly
        vLayout=new QVBoxLayout();
        this->setLayout(vLayout);


        networkGroup = new QGroupBox("Network settings");
        networkLayout = new QGridLayout();
        int i =0;

        QLabel* label= new QLabel("Vision multicast address");
        labels.push_back(label);
        visionAddress = new QLineEdit();
        visionAddress->setText(QString("127.0.1.2"));
        networkLayout->addWidget(label,i,0);
        networkLayout->addWidget(visionAddress,i,1);
        i++;

        label = new QLabel("Vision Port");
        labels.push_back(label);
        visionPort = new QSpinBox();
        visionPort->setRange(0,999999);
        visionPort->setValue(10006);
        networkLayout->addWidget(label,i,0);
        networkLayout->addWidget(visionPort,i,1);
        i++;

        label= new QLabel("Blue Commands multicast address");
        labels.push_back(label);
        blueIP = new QLineEdit();
        blueIP->setText(QString("127.0.1.3"));
        networkLayout->addWidget(label,i,0);
        networkLayout->addWidget(blueIP,i,1);
        i++;

        label = new QLabel("Blue Commands Port");
        labels.push_back(label);
        bluePort = new QSpinBox();
        bluePort->setRange(0,999999);
        bluePort->setValue(10006);
        networkLayout->addWidget(label,i,0);
        networkLayout->addWidget(bluePort,i,1);
        i++;

        label= new QLabel("Yellow Commands multicast address");
        labels.push_back(label);
        yellowIP = new QLineEdit();
        yellowIP->setText(QString("127.0.1.4"));
        networkLayout->addWidget(label,i,0);
        networkLayout->addWidget(yellowIP,i,1);
        i++;

        label = new QLabel("Yellow Commands Port");
        labels.push_back(label);
        yellowPort = new QSpinBox();
        yellowPort->setRange(0,999999);
        yellowPort->setValue(10006);
        networkLayout->addWidget(label,i,0);
        networkLayout->addWidget(yellowPort,i,1);
        i++;
        label = new QLabel("Send geometry every x ticks:");
        labels.push_back(label);
        sendGeometryTicks = new QSpinBox();
        sendGeometryTicks->setRange(0,999999);
        sendGeometryTicks->setValue(109); //TODO: fix initialization
        networkLayout->addWidget(label,i,0);
        networkLayout->addWidget(sendGeometryTicks,i,1);
        i++;
        label = new QLabel("Delay (ms)");
        labels.push_back(label);
        delay = new QDoubleSpinBox();
        delay->setRange(0.0,200.0);
        delay->setDecimals(1);
        networkLayout->addWidget(label,i,0);
        networkLayout->addWidget(delay,i,1);

        networkGroup->setLayout(networkLayout);

        controlsGroup= new QGroupBox("Simulator controls");
        controlsLayout = new QGridLayout();
        i=0;

        label = new QLabel("Blue robots");
        labels.push_back(label);
        blueBotSettings = new QComboBox();
        for (const auto &configName : simulator->getRobotConfigNames()) {
            blueBotSettings->addItem(configName);
        }
        controlsLayout->addWidget(label,i,0);
        controlsLayout->addWidget(blueBotSettings,i,1);
        i++;

        label = new QLabel("Yellow robots");
        labels.push_back(label);
        yellowBotSettings = new QComboBox();
        for (const auto &configName : simulator->getRobotConfigNames()) {
            yellowBotSettings->addItem(configName);
        }
        controlsLayout->addWidget(label,i,0);
        controlsLayout->addWidget(yellowBotSettings,i,1);
        i++;

        label = new QLabel("World Settings");
        labels.push_back(label);
        worldSettings = new QComboBox();
        for (const auto &configName : simulator->getWorldConfigNames()) {
            worldSettings->addItem(configName);
        }
        controlsLayout->addWidget(label,i,0);
        controlsLayout->addWidget(worldSettings,i,1);
        i++;

        label = new QLabel("Blue Robot Count");
        labels.push_back(label);
        blueBotCount = new QSpinBox();
        blueBotCount->setRange(0,16);
        blueBotCount->setValue(0);
        controlsLayout->addWidget(label,i,0);
        controlsLayout->addWidget(blueBotCount,i,1);
        i++;

        label = new QLabel("Yellow Robot Count");
        labels.push_back(label);
        yellowBotCount = new QSpinBox();
        yellowBotCount->setRange(0,16);
        yellowBotCount->setValue(0);
        controlsLayout->addWidget(label,i,0);
        controlsLayout->addWidget(yellowBotCount,i,1);
        i++;

        label = new QLabel("Robot X "+QString(QChar(0x03C3))+" (m)");
        labels.push_back(label);
        robotXNoise = new QDoubleSpinBox();
        robotXNoise->setRange(0,0.2);
        robotXNoise->setSingleStep(0.005);
        robotXNoise->setDecimals(3);
        controlsLayout->addWidget(label,i,0);
        controlsLayout->addWidget(robotXNoise,i,1);
        i++;

        label = new QLabel("Robot Y "+QString(QChar(0x03C3))+" (m)");
        labels.push_back(label);
        robotYNoise = new QDoubleSpinBox();
        robotYNoise->setRange(0,0.2);
        robotYNoise->setSingleStep(0.005);
        robotYNoise->setDecimals(3);
        controlsLayout->addWidget(label,i,0);
        controlsLayout->addWidget(robotYNoise,i,1);
        i++;

        label = new QLabel("Robot orientation "+QString(QChar(0x03C3))+" (rad)");
        labels.push_back(label);
        robotOrNoise = new QDoubleSpinBox();
        robotOrNoise->setRange(0,0.5);
        robotOrNoise->setSingleStep(0.005);
        robotOrNoise->setDecimals(3);
        controlsLayout->addWidget(label,i,0);
        controlsLayout->addWidget(robotOrNoise,i,1);
        i++;

        label = new QLabel("Robot Vanishing");
        labels.push_back(label);
        robotVanishing = new QDoubleSpinBox();
        robotVanishing->setRange(0,1.0);
        robotVanishing->setSingleStep(0.005);
        robotVanishing->setDecimals(3);
        controlsLayout->addWidget(label,i,0);
        controlsLayout->addWidget(robotVanishing,i,1);
        i++;

        label = new QLabel("Ball X "+QString(QChar(0x03C3))+" (m)");
        labels.push_back(label);
        ballXNoise = new QDoubleSpinBox();
        ballXNoise->setRange(0,0.2);
        ballXNoise->setSingleStep(0.005);
        ballXNoise->setDecimals(3);
        controlsLayout->addWidget(label,i,0);
        controlsLayout->addWidget(ballXNoise,i,1);
        i++;

        label = new QLabel("Ball Y "+QString(QChar(0x03C3))+" (m)");
        labels.push_back(label);
        ballYNoise = new QDoubleSpinBox();
        ballYNoise->setRange(0,0.2);
        ballYNoise->setSingleStep(0.005);
        ballYNoise->setDecimals(3);
        controlsLayout->addWidget(label,i,0);
        controlsLayout->addWidget(ballYNoise,i,1);
        i++;

        label = new QLabel("Ball Vanishing");
        labels.push_back(label);
        ballVanishing = new QDoubleSpinBox();
        ballVanishing->setRange(0,1.0);
        ballVanishing->setSingleStep(0.005);
        ballVanishing->setDecimals(3);
        controlsLayout->addWidget(label,i,0);
        controlsLayout->addWidget(ballVanishing,i,1);

        controlsGroup->setLayout(controlsLayout);

        connect(visionAddress,&QLineEdit::textChanged,simulator,&Simulator::setVisionIP);
        connect(visionPort,SIGNAL(valueChanged(int)),simulator,SLOT(setVisionPort(int)));
        connect(blueIP,&QLineEdit::textChanged,simulator,&Simulator::setBlueIP);
        connect(bluePort,SIGNAL(valueChanged(int)),simulator,SLOT(setBluePort(int)));
        connect(yellowIP,&QLineEdit::textChanged,simulator,&Simulator::setYellowIP);
        connect(yellowPort,SIGNAL(valueChanged(int)),simulator,SLOT(setYellowPort(int)));

        connect(blueBotCount, SIGNAL(valueChanged(int)),simulator,SLOT(setBlueBotCount(int)));
        connect(yellowBotCount,SIGNAL(valueChanged(int)),simulator,SLOT(setYellowBotCount(int)));

        connect(blueBotSettings,&QComboBox::currentTextChanged,simulator,&Simulator::setBlueConfig);
        connect(yellowBotSettings,&QComboBox::currentTextChanged,simulator,&Simulator::setYellowConfig);
        connect(worldSettings,&QComboBox::currentTextChanged,simulator,&Simulator::setWorldConfig);

        connect(robotXNoise,SIGNAL(valueChanged(double)),simulator,SLOT(setRobotXNoise(double)));
        connect(robotYNoise,SIGNAL(valueChanged(double)),simulator,SLOT(setRobotYNoise(double)));
        connect(robotOrNoise,SIGNAL(valueChanged(double)),simulator,SLOT(setRobotOrNoise(double)));
        connect(robotVanishing,SIGNAL(valueChanged(double)),simulator,SLOT(setRobotVanishing(double)));

        connect(ballXNoise,SIGNAL(valueChanged(double)),simulator,SLOT(setBallXNoise(double)));
        connect(ballYNoise,SIGNAL(valueChanged(double)),simulator,SLOT(setBallYNoise(double)));
        connect(ballVanishing,SIGNAL(valueChanged(double)),simulator,SLOT(setBallVanishing(double)));

        connect(delay,SIGNAL(valueChanged(double)),simulator,SLOT(setDelay(double)));
        vLayout->addWidget(controlsGroup);
        vLayout->addWidget(networkGroup);
    }
}
