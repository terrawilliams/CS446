//
// Created by terra on 3/5/19.
//

#include "Timer.h"

Timer::Timer()
{

}

Timer::~Timer()
{

}

void Timer::StartTimer()
{
    startTime = std::clock();
}

void Timer::EndTimer()
{
    endTime = std::clock();
}

double Timer::GetDuration()
{
    return ((endTime - startTime) / ((double) CLOCKS_PER_SEC));
}

void Timer::Wait(double time)
{
    clock_t start = std::clock();
    clock_t  end= std::clock();

    while(((end - start) / (double) CLOCKS_PER_SEC) < time)
    {
        end = std::clock();
    }
}

clock_t Timer::GetStartTime()
{
    return startTime;
}

clock_t Timer::GetEndTime()
{
    return endTime;
}
