//
// Created by rolf on 24-09-19.
//

#include "MainWindow.h"
#include "DebugVisualization.h"
#include "../simulator/physics/SimWorld.h" //TODO: fix path
namespace interface {
    MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
        simulator= new SimWorld();
        debugVisualization = new DebugVisualization(simulator->getWorld(),this);
        setCentralWidget(debugVisualization);
        showMaximized();
    }
}