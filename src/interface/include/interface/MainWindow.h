//
// Created by rolf on 24-09-19.
//

#ifndef ROBOTEAM_MIMIR_MAINWINDOW_H
#define ROBOTEAM_MIMIR_MAINWINDOW_H


#include <QMainWindow>
namespace interface {
    class MainWindow : public QMainWindow {
    Q_OBJECT
    public:
        explicit MainWindow(QWidget* parent = nullptr);
    };
}

#endif //ROBOTEAM_MIMIR_MAINWINDOW_H
