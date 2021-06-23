#include "Particle.hpp"

Particle::Particle(const glm::vec2 &position, const GLfloat &density, const GLfloat &mass)
    : position(position), velocity(0.f, 0.f), acceleration(0.f, 0.f),
      density(density), pressure(0.f), mass(mass)
{
}

Particle::~Particle()
{
}