//
// Created by rolf on 24-09-19.
//

#include "MainWindow.h"
#include "DebugVisualization.h"
#include "SettingsWidget.h"
#include "Simulator.h"
#include "Visualizer.h"
#include "Visualizer3D.h"
#include <thread>

namespace interface {
MainWindow::MainWindow(QWidget* parent)
        :QMainWindow(parent) {

    simulator = new Simulator();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //debugVisualization = new DebugVisualization(simulator->getWorldSettings().scale, simulator->getPhysicsWorld(), this);
    settingsWidget = new SettingsWidget(simulator, this);
    visualizer = new Visualizer(simulator->getWorldSettings(), simulator->getYellowSettings(),
            simulator->getBlueSettings(), simulator->getGeometry(), this);
    connect(simulator,&Simulator::geometryUpdated,visualizer,&Visualizer::setGeometryData);
    connect(simulator,&Simulator::sentPackets,visualizer,&Visualizer::addDetections);
    connect(simulator,&Simulator::blueUpdated,visualizer,&Visualizer::setBlueSettings);
    connect(simulator,&Simulator::yellowUpdated,visualizer,&Visualizer::setYellowSettings);
    mainLayout = new QVBoxLayout();
    hLayout = new QHBoxLayout();
//    userMenus = new QVBoxLayout();
//    userMenus->addWidget(settingsWidget);

    auto visualizer3d = new Visualizer3D(simulator->getWorldSettings().scale,this);
    visualizer3d->addDebugDrawing(simulator->getPhysicsWorld());
    visualizer3d->addVisualWorld(simulator->getVisualWorld());

    splitter = new QSplitter();
    splitter->addWidget(settingsWidget);
    splitter->addWidget(visualizer);
    //splitter->addWidget(debugVisualization);
    splitter->addWidget(visualizer3d);
    splitter->setSizes({100, 400,400,900});
    setCentralWidget(splitter);
    showMaximized();
}
}