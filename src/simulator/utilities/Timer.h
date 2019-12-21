//
// Created by rolf on 15-12-19.
//

#ifndef ROBOTEAM_MIMIR_TIMER_H
#define ROBOTEAM_MIMIR_TIMER_H


#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QArgument>

struct Timing {
    Timing(QString name, int waitTime, double timeStepSize);
    QString name;
    int millisecWaitTime;
    double timeStepLength;
    [[nodiscard]] double realTimeFactor() const;
    [[nodiscard]] double worldHz() const;
};

class Timer {
public:
    Timer();
    QList<Timing> getTimings();
    Timing getTimer(const QString &name);
private:
    QList<Timing> timings;
};

#endif //ROBOTEAM_MIMIR_TIMER_H
