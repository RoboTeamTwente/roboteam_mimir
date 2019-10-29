//
// Created by rolf on 19-09-19.
//

#include <QApplication>
#include "interface/MainWindow.h"
int main(int argc, char **argv) {

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling); //must be initialized before QApplication (see docs)
    QApplication app(argc, argv);
    interface::MainWindow* window= new interface::MainWindow(nullptr); //mainwindow owns a copy of the simulator
    window->show();
    return app.exec();
}