//
// Created by rolf on 24-09-19.
//

#ifndef ROBOTEAM_MIMIR_MAINWINDOW_H
#define ROBOTEAM_MIMIR_MAINWINDOW_H


#include <QMainWindow>
class Simulator;
namespace interface {
    class DebugVisualization;
    class MainWindow : public QMainWindow {
    Q_OBJECT
    public:
        explicit MainWindow(QWidget* parent = nullptr);
    private:
        DebugVisualization* debugVisualization;
         Simulator* simulator;
    };
}

#endif //ROBOTEAM_MIMIR_MAINWINDOW_H
