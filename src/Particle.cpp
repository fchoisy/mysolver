#include "Particle.hpp"

Particle::Particle(const ParticleSet *set, const glm::vec2 &position, const float &density, const float &volume)
    : set(set), position(position), velocity(0.f, 0.f), acceleration(0.f, 0.f),
      pressureAcceleration(0.f, 0.f), viscosityAcceleration(0.f, 0.f), otherAccelerations(0.f, 0.f),
      density(density), pressure(0.f), volume_(volume), mass_(density * volume_)
{
}

Particle::~Particle()
{
}

float Particle::mass() const
{
  return mass_;
}

float Particle::volume() const
{
  return volume_;
}