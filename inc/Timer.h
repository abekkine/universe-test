#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>
#include <time.h>

class Timer {

public:
    Timer();
    ~Timer();
    void Reset();
    void Set();
    unsigned int GetElapsed();
    bool GetTimestamp( char *timeStr, const char *fmtStr );

private:
    unsigned int current_tick_value;
    unsigned int marked_tick_value;
    struct timeval tCurrent;

    void InitVars();
    unsigned int TickValue();

};

#endif

