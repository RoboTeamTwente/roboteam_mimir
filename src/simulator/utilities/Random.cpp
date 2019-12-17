//
// Created by rolf on 17-12-19.
//

#include "Random.h"
Random::Random(double xRange, double yRange, double orRange, int xSeed, int ySeed, int orientationSeed,
        int vanishingSeed) {

    xGenerator=std::mt19937(xSeed);
    yGenerator=std::mt19937(ySeed);
    orientationGenerator=std::mt19937(orientationSeed);

    xDist=std::normal_distribution<double>(0,xRange);
    yDist=std::normal_distribution<double>(0,yRange);
    orDist=std::normal_distribution<double>(0,orRange);

    vanishGenerator=std::mt19937(vanishingSeed);
    vanishDist=std::uniform_real_distribution<double>(0,1);
}

double Random::getOrientation() {
    return orDist(orientationGenerator);
}
double Random::getX() {
    return xDist(xGenerator);
}
double Random::getY() {
    return yDist(yGenerator);
}
double Random::getVanishing() {
    return vanishDist(vanishGenerator);
}
void Random::setOrientationRange(double orientationRange) {
    orDist=std::normal_distribution<double>(0,orientationRange);
}
void Random::setXRange(double xRange) {
    xDist=std::normal_distribution<double>(0,xRange);
}
void Random::setYRange(double yRange) {
    yDist=std::normal_distribution<double>(0,yRange);
}