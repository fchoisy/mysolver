#pragma once

#include <vector>
#include <map>
#include <glm/vec2.hpp>

#include "Particle.hpp"
#include "ParticleSet.hpp"

class ParticleSimulation
{
private:
    std::vector<ParticleSet *> particleSets;
    std::map<const Particle *, std::vector<const Particle *>> neighbors;
    std::map<const Particle *, std::vector<const Particle *>> staticNeighbors;

public:
    void AddParticleSet(ParticleSet &particleSet);
    void UpdateNeighbors(float kernelSupport);
    const std::vector<const Particle *> &GetNeighbors(const Particle &particle) const;
    const std::vector<const Particle *> &GetStaticNeighbors(const Particle &particle) const;
    // Update all quantities except position and velocity
    void UpdateParticleQuantities(const glm::vec2 gravity) const;
    // Estimate best time step
    float ComputeTimeStep(float CFLNumber) const;
    // Update particles positions and velocities
    void UpdateParticlePositions(float timeStep) const;
};