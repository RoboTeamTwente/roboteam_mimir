//
// Created by rolf on 17-12-19.
//

#ifndef ROBOTEAM_MIMIR_RANDOM_H
#define ROBOTEAM_MIMIR_RANDOM_H
#include <random>

class Random {
    public:
        Random(double xRange, double yRange, double orRange, double ballXRange, double ballYRange, int xSeed = 42,
                int ySeed = 1337, int orientationSeed = 2193, int vanishingSeed = 1578, int ballXSeed = 83,
                int ballYSeed = 92);
        void setXRange(double xRange);
        void setYRange(double yRange);
        void setOrientationRange(double orientationRange);
        void setBallXRange(double xRange);
        void setBallYRange(double yRange);

        double getX();
        double getY();
        double getOrientation();
        double getVanishing();
        double getBallY();
        double getBallX();
    private:
        std::mt19937 vanishGenerator;
        std::mt19937 orientationGenerator;
        std::mt19937 xGenerator;
        std::mt19937 yGenerator;
        std::mt19937 ballXGenerator;
        std::mt19937 ballYGenerator;

        std::normal_distribution<double> xDist;
        std::normal_distribution<double> yDist;
        std::normal_distribution<double> orDist;
        std::normal_distribution<double> ballXDist;
        std::normal_distribution<double> ballYDist;

        std::uniform_real_distribution<double> vanishDist;
};

#endif //ROBOTEAM_MIMIR_RANDOM_H
