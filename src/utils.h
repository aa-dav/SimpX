#ifndef UTILS_H
#define UTILS_H

#include "QElapsedTimer"

class FpsMeter
{
    static const int measureSize = 60;
    QElapsedTimer timer;
    unsigned int fps = 0;
    int curMeasure = 0;
    qint64 startTicks = 0;
public:
    unsigned int getFps() { return fps; };
    FpsMeter();
    void tick();
};

#endif // UTILS_H
