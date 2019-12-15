//
// Created by rolf on 15-12-19.
//

#include "Timer.h"

#include <utility>

Timing::Timing(QString name, int waitTime, double timeStepSize) :
        name(std::move(name)),
        millisecWaitTime(waitTime),
        timeStepLength(timeStepSize) {}

double Timing::realTimeFactor() const {
    return 1000 * timeStepLength / millisecWaitTime;
}
double Timing::worldHz() const {
    return 1.0 / timeStepLength;
}

Timer::Timer() {
    timings = {
            Timing("Realtime 250hz", 4, 1 / 250.0),
            Timing("Realtime 500hz", 2, 1 / 500.0),
            Timing("Realtime 1000hz", 1, 1 / 1000.0),
            Timing("Doubletime 250hz", 2, 1 / 250.0),
            Timing("Doubletime 500hz", 1, 1 / 500.0),
            Timing("Quadrupletime 250hz", 1, 1 / 250.0),
            Timing("Halftime 1000hz", 2, 1 / 1000.0),
            Timing("Halftime 2000hz", 1, 1 / 2000.0),
            Timing("Quartertime 2000hz", 2, 1 / 2000.0),
            Timing("Quartertime 4000hz", 1, 1 / 4000.0),
            Timing("Quartertime 250hz",16,1/250.0)
    };
}
QList<Timing> Timer::getTimings() {
    return timings;
}
Timing Timer::getTimer(const QString& name) {
    for (auto timing : timings) {
        if (timing.name==name){
            return timing;
        }
    }
}