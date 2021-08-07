#include "HistoryTracker.hpp"

#include <glm/geometric.hpp> // glm::length, glm::max

HistoryTracker::HistoryTracker()
    : target(nullptr)
{
}

void HistoryTracker::SetTarget(const ParticleSet *target)
{
    this->target = target;
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

void HistoryTracker::Step(float currentTime)
{
    if (target != nullptr && target->particles.size() >= 1)
    {
        for (size_t i = 0; i < target->particles.size(); i++)
        {
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

std::vector<float> &HistoryTracker::GetTimeHistory()
{
    return timeHistory;
}

void HistoryTracker::Clear()
{
    timeHistory.clear();
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