
#pragma once

#include "ParticleSet.hpp"
#include <vector>

// Records the evolution of a patricle set over time.
class HistoryTracker
{
public:
    HistoryTracker();
    void SetTarget(const ParticleSet *target);
    void Step(float currentTime);
    void Clear();
    std::vector<float> &GetTimeHistory();
    // Properties of particle at index 0 over time
    std::vector<std::vector<float>> density;
    std::vector<std::vector<float>> pressure;
    std::vector<std::vector<float>> pressureAcceleration;
    std::vector<std::vector<float>> viscosityAcceleration;
    std::vector<std::vector<float>> otherAccelerations;
    std::vector<std::vector<float>> velocity;
    std::vector<float> maxDistance;

private:
    std::vector<float> timeHistory;
    const ParticleSet *target;
};
