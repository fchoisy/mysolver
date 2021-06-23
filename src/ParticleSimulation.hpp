#pragma once

#include <vector>
#include <map>
#include <glm/vec2.hpp>
// #include <functional>  // reference_wrapper

#include "Particles.hpp"

class ParticleSimulation
{
private:
    std::vector<ParticleSet *> particleSets;
    std::map<const Particle *, std::vector<const Particle *>> neighbors;

public:
    void AddParticleSet(ParticleSet &particleSet);
    void UpdateNeighbors(float kernelSupport);
    const std::vector<const Particle *> &GetNeighbors(const Particle &particle) const;
    void UpdateParticles(float timeStep, const glm::vec2 gravity) const;
};