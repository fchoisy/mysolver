#include "ParticleSimulation.hpp"

#include <glm/geometric.hpp>
#include "Kernel.hpp"

#include <iostream> // DEBUG

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
            staticNeighbors[&particle] = std::vector<const Particle *>();
            for (auto &&otherParticleSet : particleSets)
            {
                auto neighborMap = &neighbors;
                if (particleSet->isStatic)
                {
                    neighborMap = &staticNeighbors;
                }
                for (auto &&otherParticle : otherParticleSet->particles)
                {
                    if (glm::distance(particle.position, otherParticle.position) < kernelSupport)
                    {
                        neighborMap->at(&particle).push_back(&otherParticle);
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

const std::vector<const Particle *> &ParticleSimulation::GetStaticNeighbors(const Particle &particle) const
{
    return staticNeighbors.at(&particle);
}

void ParticleSimulation::UpdateParticles(const float timeStep, const glm::vec2 gravity) const
{
    for (auto &&particleSet : particleSets)
    {
        if (!particleSet->isStatic)
        {
            Kernel kernel(particleSet->spacing);
            // Compute density and pressure for each particle
            for (auto &&particle : particleSet->particles)
            {
                particle.density = 0.f;
                for (auto &&neighbor : GetNeighbors(particle))
                {
                    particle.density += neighbor->mass * kernel.Function(particle.position, neighbor->position);
                }
                particle.pressure = glm::max(particleSet->stiffness * (particle.density / particleSet->restDensity - 1.f), 0.f);
            }

            // Compute accelerations for each particle
            for (auto &&particle : particleSet->particles)
            {
                // Viscosity acceleration
                glm::vec2 viscosityAcceleration(0.f, 0.f);
                for (auto &&neighbor : GetNeighbors(particle))
                {
                    glm::vec2 positionDiff = neighbor->position - particle.position;
                    glm::vec2 velocityDiff = neighbor->velocity - particle.velocity;
                    glm::vec2 kernelDer = kernel.Derivative(particle.position, neighbor->position);
                    viscosityAcceleration +=
                        kernelDer *
                        (neighbor->mass / neighbor->density) *
                        (glm::dot(velocityDiff, positionDiff)) /
                        (glm::dot(positionDiff, positionDiff) + 0.01f * particleSet->spacing * particleSet->spacing);
                }
                viscosityAcceleration *= 2.f; // dimensionality (2D)
                viscosityAcceleration *= particleSet->viscosity;
                // Pressure acceleration from fluid particles
                glm::vec2 fluidPressureAcceleration(0.f, 0.f);
                for (auto &&neighbor : GetNeighbors(particle))
                {
                    fluidPressureAcceleration += (particle.pressure / (particle.density * particle.density) + neighbor->pressure / (neighbor->density * neighbor->density)) * kernel.Derivative(particle.position, neighbor->position);
                }
                fluidPressureAcceleration *= -particle.mass;
                // Pressure acceleration from (static) boundary particles
                glm::vec2 boundaryPressureAcceleration(0.f, 0.f);
                for (auto &&staticNeighbor : GetStaticNeighbors(particle))
                {
                    boundaryPressureAcceleration += kernel.Derivative(particle.position, staticNeighbor->position);
                }
                boundaryPressureAcceleration *= -2 * particle.mass * particle.pressure / (particle.density * particle.density);
                // Total pressure acceleration
                glm::vec2 pressureAcceleration = fluidPressureAcceleration + boundaryPressureAcceleration;
                // Other accelerations
                glm::vec2 otherAccelerations = gravity;
                // Total acceleration
                particle.acceleration = viscosityAcceleration + pressureAcceleration + otherAccelerations;
            }
            // Update position based on acceleration for each particle
            // using the semi-implicit Euler method
            for (auto &&particle : particleSet->particles)
            { 
                particle.velocity += timeStep * particle.acceleration;
                particle.position += timeStep * particle.velocity;
            }
        }
    }
}