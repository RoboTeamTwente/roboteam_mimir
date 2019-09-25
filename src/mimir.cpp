//
// Created by rolf on 19-09-19.
//

#include "simulator/include/simulator/SimWorld.h"
#include <QApplication>
#include "interface/MainWindow.h"
int main(int argc, char **argv) {

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling); //must be initialized before QApplication (see docs)
    QApplication app(argc, argv);
    interface::MainWindow* window= new interface::MainWindow(nullptr);
    window->show();
    return app.exec();
}