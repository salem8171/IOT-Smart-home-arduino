#include <Arduino.h>

#ifndef ASYNCHRONOUS_DELAYER
#define ASYNCHRONOUS_DELAYER

class AsynchronousDelayer
{
    private:
        unsigned long time_to_wait;
        unsigned long start_time;
        bool elapsed = true;

    public:
        void startTimer(unsigned long);
        bool hasElapsed();
        void restartTimer(unsigned long);
        void stopTimer();
        bool coolDownElapsed();
};

void AsynchronousDelayer::startTimer(unsigned long time_to_wait)
{
    if (elapsed == false) return;
    restartTimer(time_to_wait);
}

bool AsynchronousDelayer::hasElapsed()
{
    if (elapsed == true) return false;
    return coolDownElapsed();
}

void AsynchronousDelayer::restartTimer(unsigned long time_to_wait)
{
    this -> time_to_wait = time_to_wait;
    start_time = millis();
    elapsed = false;
}

void AsynchronousDelayer::stopTimer()
{
    elapsed = true;
}

bool AsynchronousDelayer::coolDownElapsed()
{
    if (millis() - start_time > time_to_wait) elapsed = true;
    return elapsed;
}

#endif