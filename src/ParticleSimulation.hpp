#pragma once

#include <vector>
#include <map>
// #include <functional>  // reference_wrapper

#include "Particles.hpp"

class ParticleSimulation
{
private:
    std::vector<const ParticleSet *> particleSets;
    std::map<const Particle *, std::vector<const Particle *>> neighbors;

public:
    // ParticleSimulation();
    void AddParticleSet(const ParticleSet &particleSet);
    void UpdateNeighbors(float kernelSupport);
    std::vector<const Particle *> &GetNeighbors(const Particle &particle);
};