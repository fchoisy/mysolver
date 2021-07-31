#pragma once

#include "ParticleSet.hpp"
#include <vector>
#include <glm/geometric.hpp> // glm::length, glm::max

class HistoryTracker
{
private:
    std::vector<float> timeHistory;
    // std::vector<std::vector<Particle>> targetHistory;
    const ParticleSet *target;

public:
    // Properties of particle at index 0 over time
    std::vector<std::vector<float>> density;
    std::vector<std::vector<float>> pressure;
    std::vector<std::vector<float>> pressureAcceleration;
    std::vector<std::vector<float>> viscosityAcceleration;
    std::vector<std::vector<float>> otherAccelerations;
    std::vector<std::vector<float>> velocity;
    std::vector<float> maxDistance;

    HistoryTracker()
        : target(nullptr)
    {
    }

    void SetTarget(const ParticleSet *target)
    {
        this->target = target;
        // for (auto &&particle : target->particles)
        density.resize(target->particles.size());
        pressure.resize(target->particles.size());
        pressureAcceleration.resize(target->particles.size());
        viscosityAcceleration.resize(target->particles.size());
        otherAccelerations.resize(target->particles.size());
        velocity.resize(target->particles.size());
        for (size_t i = 0; i < target->particles.size(); i++)
        {
            density[i] = std::vector<float>();
            pressure[i] = std::vector<float>();
            pressureAcceleration[i] = std::vector<float>();
            viscosityAcceleration[i] = std::vector<float>();
            otherAccelerations[i] = std::vector<float>();
            velocity[i] = std::vector<float>();
        }
        maxDistance.clear();
    }

    void Step(float currentTime)
    {
        if (target != nullptr && target->particles.size() >= 1)
        {
            // for (auto &&particle : target->particles)
            for (size_t i = 0; i < target->particles.size(); i++)
            {
                // targetHistory.push_back(target->particles);
                // auto frontParticle = target->particles.front();
                auto particle = target->particles[i];
                density[i].push_back(particle.density / 1000.f);
                pressure[i].push_back(particle.pressure / 10000.f);
                pressureAcceleration[i].push_back(glm::length(particle.pressureAcceleration) / 10.f);
                viscosityAcceleration[i].push_back(glm::length(particle.viscosityAcceleration) / 10.f);
                otherAccelerations[i].push_back(glm::length(particle.otherAccelerations) / 10.f);
                velocity[i].push_back(glm::length(particle.velocity));
            }
            float newMaxDistance = 0.f;
            if (timeHistory.size() >= 1)
            {
                float timeStep = currentTime - timeHistory.back();
                for (auto &&v : velocity)
                {
                    newMaxDistance = glm::max(newMaxDistance, timeStep * v.back());
                }
            }
            maxDistance.push_back(newMaxDistance);
            timeHistory.push_back(currentTime);
        }
    }

    std::vector<float> &GetTimeHistory()
    {
        return timeHistory;
    }

    // std::vector<std::vector<Particle>> &GetTargetHistory()
    // {
    //     return targetHistory;
    // }

    // std::map<const Particle *, std::map<std::string, std::vector<float>>> &GetPropertiesHistory()
    // {
    //     return propertiesHistory;
    // }

    void Clear()
    {
        timeHistory.clear();
        // targetHistory.clear();
        // propertiesHistory.clear();
        for (size_t i = 0; i < target->particles.size(); i++)
        {
            density[i].clear();
            pressure[i].clear();
            pressureAcceleration[i].clear();
            viscosityAcceleration[i].clear();
            otherAccelerations[i].clear();
            velocity[i].clear();
        }
        maxDistance.clear();
    }
};
