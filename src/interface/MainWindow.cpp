//
// Created by rolf on 24-09-19.
//

#include "MainWindow.h"
#include "DebugVisualization.h"
namespace interface {
    MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
        debugVisualization = new DebugVisualization(this);
        setCentralWidget(debugVisualization);
        showMaximized();
    }
}