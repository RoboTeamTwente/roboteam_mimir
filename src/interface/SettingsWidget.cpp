//
// Created by rolf on 27-11-19.
//

#include "SettingsWidget.h"
namespace interface{
    SettingsWidget::SettingsWidget(QWidget *parent) : QWidget(parent){
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

        networkGroup->setLayout(networkLayout);

        controlsGroup= new QGroupBox("Simulator controls");
        controlsLayout = new QGridLayout();
        i=0;

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

        controlsGroup->setLayout(controlsLayout);

        vLayout->addWidget(controlsGroup);
        vLayout->addWidget(networkGroup);
    }
}
