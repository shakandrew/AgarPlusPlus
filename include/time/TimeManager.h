#ifndef AGARPLUSPLUS_TIMEMANAGER_H
#define AGARPLUSPLUS_TIMEMANAGER_H

class TimeManager
{
    double deltaTime;

    double lastFrameStartTime;

    double tick;

#if _WIN32
    LARGE_INTEGER startTime = { 0 };

    double performanceCountDuration;
#else
    std::chrono::high_resolution_clock::time_point startTime;
#endif

public:

    TimeManager();

    void advance();

    double getDeltaTime() const;

    double getTicks() const;

    double getTime() const;

    double getTimeSinceLastTick() const;
};

#endif
