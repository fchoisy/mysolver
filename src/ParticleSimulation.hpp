#pragma once

#include <vector>
#include <map>
#include <glm/vec2.hpp>
// #include <functional>  // reference_wrapper

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
    void UpdateParticles(float timeStep, const glm::vec2 gravity) const;
};