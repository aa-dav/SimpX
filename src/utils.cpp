#include "utils.h"

FpsMeter::FpsMeter()
{
    timer.start();
    startTicks = timer.elapsed();
}

void FpsMeter::tick()
{
    curMeasure++;
    if ( curMeasure >= measureSize )
    {
        qint64 endTicks = timer.elapsed();
        if ( endTicks - startTicks > 0 )
        {
            fps = measureSize * 1000 / (endTicks - startTicks);
        }
        else
        {
            fps = 0;
        }
        curMeasure = 0;
        startTicks = endTicks;
    }
}
