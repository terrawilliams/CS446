//
// Created by terra on 3/5/19.
//

#ifndef PROJECT1_TIMER_H
#define PROJECT1_TIMER_H

#include <ctime>


class Timer
{
public:
    Timer();
    ~Timer();

    void StartTimer();
    void EndTimer();

    double GetDuration();
    clock_t GetStartTime();
    clock_t GetEndTime();

    void Wait(double time);

private:
    std::clock_t startTime;
    std::clock_t endTime;
    double duration;
};


#endif //PROJECT1_TIMER_H
