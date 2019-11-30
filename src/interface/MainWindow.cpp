//
// Created by rolf on 24-09-19.
//

#include "MainWindow.h"
#include "DebugVisualization.h"
#include "SettingsWidget.h"
#include "simulator/Simulator.h"
#include "Visualizer.h"

namespace interface {
MainWindow::MainWindow(QWidget* parent)
        :QMainWindow(parent) {

    simulator = new Simulator();
    debugVisualization = new DebugVisualization(simulator->getWorldSettings(), simulator->getPhysicsWorld(), this);
    settingsWidget = new SettingsWidget(simulator, this);
    visualizer = new Visualizer(simulator->getWorldSettings().get(), simulator->getYellowSettings().get(),
            simulator->getBlueSettings().get(), simulator->getGeometry(), this);
    connect(simulator,&Simulator::geometryUpdated,visualizer,&Visualizer::setGeometryData);
    connect(simulator,&Simulator::sentPackets,visualizer,&Visualizer::addDetections);
    mainLayout = new QVBoxLayout();
    hLayout = new QHBoxLayout();
//    userMenus = new QVBoxLayout();
//    userMenus->addWidget(settingsWidget);

    splitter = new QSplitter();
    splitter->addWidget(settingsWidget);
    splitter->addWidget(visualizer);
    splitter->addWidget(debugVisualization);
    splitter->setSizes({100, 900,900});
    setCentralWidget(splitter);
    showMaximized();
}
}