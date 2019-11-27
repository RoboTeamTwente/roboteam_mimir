//
// Created by rolf on 27-11-19.
//

#ifndef ROBOTEAM_MIMIR_SETTINGSWIDGET_H
#define ROBOTEAM_MIMIR_SETTINGSWIDGET_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>

namespace interface{
    class SettingsWidget : public QWidget{
    Q_OBJECT

    private:
        QList<QLabel *> labels;
        QVBoxLayout *vLayout;
        QGroupBox *networkGroup;
        QGridLayout *networkLayout;
        QLineEdit *visionAddress;
        QSpinBox *visionPort;
        QLineEdit *blueIP;
        QSpinBox *bluePort;
        QLineEdit *yellowIP;
        QSpinBox * yellowPort;

        QGroupBox *controlsGroup;
        QGridLayout *controlsLayout;
        QSpinBox *blueBotCount;
        QSpinBox *yellowBotCount;

    public:
        explicit SettingsWidget(QWidget* parent = nullptr);

    };
}

#endif //ROBOTEAM_MIMIR_SETTINGSWIDGET_H
