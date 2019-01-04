#include "Timer.h"

Timer::Timer()
{
    InitVars();
    Reset();
}

Timer::~Timer()
{
}

void Timer::InitVars()
{
    current_tick_value = 0;
    marked_tick_value = 0;
}

unsigned int Timer::TickValue()
{
    unsigned int value;

    gettimeofday( &tCurrent, 0 );

    value = tCurrent.tv_sec * 1000000 + tCurrent.tv_usec;

    return value;
}

void Timer::Reset()
{
    current_tick_value = TickValue();
    marked_tick_value = current_tick_value;
}

void Timer::Set()
{
    marked_tick_value = TickValue();
}

unsigned int Timer::GetElapsed()
{
    return TickValue() - marked_tick_value;
}

bool Timer::GetTimestamp( char *timeStr, const char *fmtStr )
{
    bool result;
    int rc;
    struct tm *ts;
    struct timeval tv;

    gettimeofday( &tv, NULL );
    ts = localtime( &tv.tv_sec );

    rc = strftime( timeStr, 256, fmtStr, ts );
    
    if( rc > 0 )
    {
        result = true;
    }
    else
    {
        result = false;
    }

    return result;
}
