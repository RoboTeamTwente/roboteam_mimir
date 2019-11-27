//
// Created by rolf on 24-09-19.
//

#ifndef ROBOTEAM_MIMIR_MAINWINDOW_H
#define ROBOTEAM_MIMIR_MAINWINDOW_H


#include <QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QSplitter>

class Simulator;
namespace interface {
    class DebugVisualization;

    class SettingsWidget;

    class MainWindow : public QMainWindow {
    Q_OBJECT
    public:
        explicit MainWindow(QWidget *parent = nullptr);
        //~MainWindow();
    private:
        QVBoxLayout *mainLayout;
        QHBoxLayout *hLayout;
        QVBoxLayout *userMenus;
        QSplitter *splitter;

        DebugVisualization *debugVisualization;
        Simulator *simulator;
        SettingsWidget *settingsWidget;
    };
}

#endif //ROBOTEAM_MIMIR_MAINWINDOW_H
