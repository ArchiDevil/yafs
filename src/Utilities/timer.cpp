#include "timer.h"

#if defined (WIN32) || (_WIN32)
#include <Windows.h>
#endif

#if defined (WIN32) || (_WIN32)

windows_high_reference_timer::windows_high_reference_timer()
{
    QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
    secPerCount = 1.0 / countsPerSec;
    QueryPerformanceCounter((LARGE_INTEGER*)&startCount);
    tickCount = startCount;
}

void windows_high_reference_timer::start()
{
    if (stopped)
    {
        QueryPerformanceCounter((LARGE_INTEGER*)&startCount);
        stopped = false;
        this->tick();
    }
}

void windows_high_reference_timer::tick()
{
    if (!stopped)
    {
        QueryPerformanceCounter((LARGE_INTEGER*)&tickCount);
    }
}

void windows_high_reference_timer::stop()
{
    if (!stopped)
    {
        QueryPerformanceCounter((LARGE_INTEGER *)&stopCount);
        stopped = true;
    }
}

double windows_high_reference_timer::get_stopped_time() const
{
    if (stopped)
    {
        long long temp;
        QueryPerformanceCounter((LARGE_INTEGER*)&temp);
        return (double)(temp - stopCount) / countsPerSec;
    }
    else
    {
        return (double)(startCount - stopCount) / countsPerSec;
    }
}

double windows_high_reference_timer::get_running_time() const
{
    if (!stopped)
    {
        long long tempCount;
        QueryPerformanceCounter((LARGE_INTEGER*)&tempCount);
        return (double)(tempCount - startCount) / countsPerSec;
    }
    return 0.0f;
}

double windows_high_reference_timer::get_delta_time() const
{
    if (!stopped)
    {
        long long tempCount;
        QueryPerformanceCounter((LARGE_INTEGER*)&tempCount);
        return (double)(tempCount - tickCount) / countsPerSec;
    }
    return 0.0f;    //lol'd
}

#endif
