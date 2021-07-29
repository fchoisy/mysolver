#pragma once

#include "ParticleSet.hpp"
#include <vector>

class HistoryTracker
{
private:
    std::vector<float> timeHistory;
    std::vector<std::vector<Particle>> targetHistory;
    const ParticleSet *target;

public:
    HistoryTracker()
        : target(nullptr)
    {
    }

    void SetTarget(const ParticleSet *target)
    {
        this->target = target;
    }

    void Step(float currentTime)
    {
        if (target != nullptr)
        {
            timeHistory.push_back(currentTime);
            targetHistory.push_back(target->particles);
        }
    }

    std::vector<float> &GetTimeHistory()
    {
        return timeHistory;
    }

    std::vector<std::vector<Particle>> &GetTargetHistory()
    {
        return targetHistory;
    }

    void Clear()
    {
        timeHistory.clear();
        targetHistory.clear();
    }
};
