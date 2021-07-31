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
    // Properties of particle at index 0 over time
    std::vector<float> density;
    std::vector<float> pressure;
    std::vector<float> pressureAcceleration;
    std::vector<float> viscosityAcceleration;
    std::vector<float> otherAccelerations;
    std::vector<float> velocity;

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
            if (target->particles.size() >= 1)
            {
                timeHistory.push_back(currentTime);
                targetHistory.push_back(target->particles);
                auto frontParticle = target->particles.front();
                density.push_back(frontParticle.density / 1000.f);
                pressure.push_back(frontParticle.pressure / 10000.f);
                pressureAcceleration.push_back(glm::length(frontParticle.pressureAcceleration) / 10.f);
                viscosityAcceleration.push_back(glm::length(frontParticle.viscosityAcceleration) / 10.f);
                otherAccelerations.push_back(glm::length(frontParticle.otherAccelerations) / 10.f);
                velocity.push_back(glm::length(frontParticle.velocity));
            }
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
        density.clear();
        pressure.clear();
        pressureAcceleration.clear();
        viscosityAcceleration.clear();
        otherAccelerations.clear();
        velocity.clear();
    }
};
