#ifdef _WIN32
	#include <Windows.h>
#else
    #include <chrono>
#endif

#include "time/TimeManager.h"

TimeManager::TimeManager()
    : tick{0}
{
    #if _WIN32
        LARGE_INTEGER performanceFrequency;
        QueryPerformanceFrequency(&performanceFrequency);
        performanceCountDuration = 1.0 / performanceFrequency.QuadPart;
        QueryPerformanceCounter(&startTime);
        lastFrameStartTime = getTime();
    #else
        startTime = std::chrono::high_resolution_clock::now();
    #endif
}

void TimeManager::advance()
{
    double currentTime = getTime();

    deltaTime = static_cast<double>(currentTime - lastFrameStartTime);

    lastFrameStartTime = currentTime;

    tick += 1;
}

double TimeManager::getDeltaTime() const
{
    return deltaTime;
}

double TimeManager::getTime() const
{
    #if _WIN32
        LARGE_INTEGER currentTime, timeSinceStart;
        QueryPerformanceCounter(&currentTime);
        timeSinceStart.QuadPart = currentTime.QuadPart - startTime.QuadPart;
        return timeSinceStart.QuadPart * performanceCountDuration;
    #else
        auto now = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
        return static_cast<double>(ms) / 1000;
    #endif
}

double TimeManager::getTicks() const
{
    return tick;
}

double TimeManager::getTimeSinceLastTick() const
{
    return (getTime() - lastFrameStartTime);
}
