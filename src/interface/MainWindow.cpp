//
// Created by rolf on 24-09-19.
//

#include "MainWindow.h"
#include "DebugVisualization.h"
#include "simulator/Simulator.h"
namespace interface {
    MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
        simulator=new Simulator();
        debugVisualization = new DebugVisualization(simulator->getPhysicsWorld(),this);
        setCentralWidget(debugVisualization);
        showMaximized();
    }
}