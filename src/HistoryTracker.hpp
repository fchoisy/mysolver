#pragma once

#include "ParticleSet.hpp"
#include <vector>

class HistoryTracker
{
private:
    std::vector<float> timeHistory;
    std::vector<std::vector<Particle>> targetHistory;
    // std::vector<float> densityHistory;
    // std::vector<float> pressureHistory;
    // std::vector<float> pressureAccelerationHistory;
    // std::vector<float> viscosityAccelerationHistory;
    // std::vector<float> otherAccelerationsHistory;
    // std::vector<float> velocityHistory;
    // std::map<const Particle *, std::map<std::string, std::vector<float>>> propertiesHistory;
    const ParticleSet *target;

public:
    HistoryTracker()
        : target(nullptr)
    {
    }

    void SetTarget(const ParticleSet *target)
    {
        this->target = target;
        // for (auto &&particle : target->particles)
        // {
        //     propertiesHistory[&particle] = std::map<std::string, std::vector<float>>();
        // }
    }

    void Step(float currentTime)
    {
        if (target != nullptr)
        {
            timeHistory.push_back(currentTime);
            targetHistory.push_back(target->particles);

            // for (auto &&particle : target->particles)
            // {
            //     propertiesHistory[&particle]["density"].push_back(3.f);
            // }
            // densityHistory.push_back(ps.at(0).density / 1000.f);
            // pressureHistory.push_back(ps.at(0).pressure / 10000.f);
            // pressureAccelerationHistory.push_back(glm::length(ps.at(0).pressureAcceleration) / 10.f);
            // viscosityAccelerationHistory.push_back(glm::length(ps.at(0).viscosityAcceleration) / 10.f);
            // otherAccelerationsHistory.push_back(glm::length(ps.at(0).otherAccelerations) / 10.f);
            // velocityHistory.push_back(glm::length(ps.at(0).velocity));
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

    // std::map<const Particle *, std::map<std::string, std::vector<float>>> &GetPropertiesHistory()
    // {
    //     return propertiesHistory;
    // }

    void Clear()
    {
        timeHistory.clear();
        targetHistory.clear();
        // propertiesHistory.clear();
    }
};
