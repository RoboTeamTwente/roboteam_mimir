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
#include <QtWidgets/QComboBox>

class Simulator;
namespace interface{
    class SettingsWidget : public QWidget{
    Q_OBJECT
    public:
        explicit SettingsWidget(Simulator* widget ,QWidget* parent = nullptr);
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
        QSpinBox * sendGeometryTicks;

        QGroupBox *controlsGroup;
        QGridLayout *controlsLayout;
        QSpinBox *blueBotCount;
        QSpinBox *yellowBotCount;
        QComboBox *blueBotSettings;
        QComboBox *yellowBotSettings;
        QComboBox *worldSettings;
        QDoubleSpinBox *robotXNoise;
        QDoubleSpinBox *robotYNoise;
        QDoubleSpinBox *robotOrNoise;
        QDoubleSpinBox *robotVanishing;
        QDoubleSpinBox *ballXNoise;
        QDoubleSpinBox *ballYNoise;
        QDoubleSpinBox *ballVanishing;

        QDoubleSpinBox *delay;

    };
}

#endif //ROBOTEAM_MIMIR_SETTINGSWIDGET_H
