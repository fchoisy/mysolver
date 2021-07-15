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

void ParticleSimulation::UpdateParticleQuantities(const glm::vec2 gravity) const
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
                    particle.density += kernel.Function(particle.position, neighbor->position);
                }
                for (auto &&neighbor : GetStaticNeighbors(particle))
                {
                    particle.density += kernel.Function(particle.position, neighbor->position);
                }
                particle.density *= particle.mass();
                particle.pressure = glm::max(particleSet->stiffness * (particle.density / particleSet->restDensity - 1.f), 0.f);
            }

            // Compute accelerations for each particle
            for (auto &&particle : particleSet->particles)
            {
                // Viscosity acceleration
                glm::vec2 fluidViscosityAcceleration(0.f, 0.f);
                for (auto &&neighbor : GetNeighbors(particle))
                {
                    glm::vec2 positionDiff = particle.position - neighbor->position;
                    glm::vec2 velocityDiff = particle.velocity - neighbor->velocity;
                    glm::vec2 kernelDer = kernel.Derivative(particle.position, neighbor->position);
                    fluidViscosityAcceleration +=
                        kernelDer *
                        neighbor->volume() *
                        (glm::dot(velocityDiff, positionDiff)) /
                        (glm::dot(positionDiff, positionDiff) + 0.01f * particleSet->spacing * particleSet->spacing);
                }
                fluidViscosityAcceleration *= 2.f;
                fluidViscosityAcceleration *= particleSet->viscosity;
                glm::vec2 staticViscosityAcceleration(0.f, 0.f);
                for (auto &&neighbor : GetStaticNeighbors(particle))
                {
                    glm::vec2 positionDiff = particle.position - neighbor->position;
                    glm::vec2 velocityDiff = particle.velocity - neighbor->velocity;
                    glm::vec2 kernelDer = kernel.Derivative(particle.position, neighbor->position);
                    staticViscosityAcceleration +=
                        neighbor->set->viscosity *
                        kernelDer *
                        neighbor->volume() *
                        (glm::dot(velocityDiff, positionDiff)) /
                        (glm::dot(positionDiff, positionDiff) + 0.01f * particleSet->spacing * particleSet->spacing);
                }
                staticViscosityAcceleration *= 2.f;
                glm::vec2 viscosityAcceleration = fluidViscosityAcceleration + staticViscosityAcceleration;
                // Pressure acceleration from fluid particles
                glm::vec2 fluidPressureAcceleration(0.f, 0.f);
                for (auto &&neighbor : GetNeighbors(particle))
                {
                    fluidPressureAcceleration += (particle.pressure / (particle.density * particle.density) + neighbor->pressure / (neighbor->density * neighbor->density)) * kernel.Derivative(particle.position, neighbor->position);
                }
                // fluidPressureAcceleration *= -particle.mass;
                // Pressure acceleration from (static) boundary particles
                glm::vec2 boundaryPressureAcceleration(0.f, 0.f);
                for (auto &&staticNeighbor : GetStaticNeighbors(particle))
                {
                    boundaryPressureAcceleration += kernel.Derivative(particle.position, staticNeighbor->position);
                }
                boundaryPressureAcceleration *= particle.pressure *
                                                (1.f / (particle.density * particle.density) +
                                                 1.f / (particleSet->restDensity * particleSet->restDensity));
                // Total pressure acceleration
                glm::vec2 pressureAcceleration = -particle.mass() * (fluidPressureAcceleration + boundaryPressureAcceleration);
                // Other accelerations
                glm::vec2 otherAccelerations = gravity;
                // Total acceleration
                particle.pressureAcceleration = pressureAcceleration;
                particle.viscosityAcceleration = viscosityAcceleration;
                particle.otherAccelerations = otherAccelerations;
                particle.acceleration = viscosityAcceleration + pressureAcceleration + otherAccelerations;
            }
        }
    }
}

float ParticleSimulation::ComputeTimeStep(float CFLNumber) const
{
    float timeStep = 0.01f; // max time step
    for (auto &&particleSet : particleSets)
    {
        if (!particleSet->isStatic)
        {
            float maxVelocity = 0.f;
            for (auto &&particle : particleSet->particles)
            {
                float velocityMagnitude = glm::length(particle.velocity);
                if (velocityMagnitude > maxVelocity)
                {
                    maxVelocity = velocityMagnitude;
                }
            }
            timeStep = glm::clamp(CFLNumber * particleSet->spacing / maxVelocity, 0.000001f, timeStep);
        }
    }
    return timeStep;
}

void ParticleSimulation::UpdateParticlePositions(float timeStep) const
{
    for (auto &&particleSet : particleSets)
    {
        if (!particleSet->isStatic)
        {
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