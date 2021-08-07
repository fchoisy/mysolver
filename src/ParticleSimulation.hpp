#pragma once

#include "Particle.hpp"
#include "ParticleSet.hpp"
#include <glm/vec2.hpp> // glm::vec2
#include <vector>
#include <map>

// Simulates fluid dynamics for a scene composed of particle sets.
class ParticleSimulation
{

public:
    // Adds a particle set to the scene
    void AddParticleSet(ParticleSet &particleSet);
    // Deletes all particle sets from scene and forgets all neighbor mappings.
    void Clear();
    // Map each particle to its nearest neighbors within a radius of `kernelSupport'.
    void UpdateNeighbors(float kernelSupport);
    // Getters for neighbors
    const std::vector<const Particle *> &GetNeighbors(const Particle &particle) const;
    const std::vector<const Particle *> &GetBoundaryNeighbors(const Particle &particle) const;
    // Update all quantities except position and velocity
    void UpdateParticleQuantities(const glm::vec2 gravity) const;
    // Estimate best time step (not used at the moment)
    float ComputeTimeStep(float CFLNumber) const;
    // Update particles positions and velocities
    void UpdateParticlePositions(float timeStep) const;

private:
    std::vector<ParticleSet *> particleSets;
    std::map<const Particle *, std::vector<const Particle *>> neighbors;
    std::map<const Particle *, std::vector<const Particle *>> boundaryNeighbors;
};