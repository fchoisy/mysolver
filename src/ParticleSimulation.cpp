#include "ParticleSimulation.hpp"

#include <glm/geometric.hpp>

// ParticleSimulation::ParticleSimulation()
// {
// }

void ParticleSimulation::UpdateNeighbors(const float kernelSupport)
{
    for (auto &&particleSet : particleSets)
    {
        for (auto &&particle : particleSet->particles)
        {
            neighbors[&particle] = std::vector<const Particle *>();
            for (auto &&otherParticleSet : particleSets)
            {
                for (auto &&otherParticle : otherParticleSet->particles)
                {
                    if (glm::distance(particle.position, otherParticle.position) < kernelSupport)
                    {
                        neighbors.at(&particle).push_back(&otherParticle);
                    }
                }
            }
        }
    }
}

std::vector<const Particle *> &ParticleSimulation::GetNeighbors(const Particle &particle)
{
    return neighbors.at(&particle);
}

void ParticleSimulation::AddParticleSet(const ParticleSet &particleSet)
{
    particleSets.push_back(&particleSet);
}