#include "ParticleSimulation.hpp"

#include <glm/geometric.hpp>
#include "Kernel.hpp"

void ParticleSimulation::AddParticleSet(ParticleSet &particleSet)
{
    particleSets.push_back(&particleSet);
}

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

const std::vector<const Particle *> &ParticleSimulation::GetNeighbors(const Particle &particle) const
{
    return neighbors.at(&particle);
}

void ParticleSimulation::UpdateParticles(const float timeStep, const glm::vec2 gravity) const
{
    for (auto &&particleSet : particleSets)
    {
        for (auto &&particle : particleSet->particles)
        {
            particle.density = 0.f;
            for (auto &&neighbor : GetNeighbors(particle))
            {
                particle.density += neighbor->mass * KernelFunction(particle.position, neighbor->position, particleSet->spacing);
            }
            particle.pressure = glm::max(particleSet->stiffness * (particle.density / particleSet->restDensity - 1.f), 0.f);
        }

        for (auto &&particle : particleSet->particles)
        {
            glm::vec2 viscosityAcceleration(0.f, 0.f);
            for (auto &&neighbor : GetNeighbors(particle))
            {
                glm::vec2 positionDiff = neighbor->position - particle.position;
                glm::vec2 velocityDiff = neighbor->velocity - particle.velocity;
                viscosityAcceleration += (neighbor->mass / neighbor->density) * (velocityDiff * positionDiff) / (glm::dot(positionDiff, positionDiff) + 0.01f * particleSet->spacing * particleSet->spacing) * KernelDerivative(particle.position, neighbor->position, particleSet->spacing);
            }
            viscosityAcceleration *= 2.f; // dimensionality (2D)
            viscosityAcceleration *= particleSet->viscosity;

            glm::vec2 pressureAcceleration(0.f, 0.f);
            for (auto &&neighbor : GetNeighbors(particle))
            {
                pressureAcceleration += neighbor->mass * (particle.pressure / (particle.density * particle.density) + neighbor->pressure / (neighbor->density * neighbor->density)) * KernelDerivative(particle.position, neighbor->position, particleSet->spacing);
            }
            pressureAcceleration *= -1.f;

            particle.acceleration = viscosityAcceleration + gravity + pressureAcceleration;
        }

        for (auto &&particle : particleSet->particles)
        {
            particle.velocity += timeStep * particle.acceleration;
            particle.position += timeStep * particle.velocity;
        }
    }
}