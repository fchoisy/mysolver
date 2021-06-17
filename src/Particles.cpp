
#include <iostream>
#include "Particles.hpp"

Particle::Particle(const glm::vec2 &position, const GLfloat &density, const GLfloat &mass)
    : position(position), velocity(0.f, 0.f), acceleration(0.f, 0.f),
      density(density), pressure(0.f), mass(mass)
{
}

Particle::~Particle()
{
}

ParticleSet::ParticleSet(int xCount, int yCount, GLfloat spacing)
    : particles(), spacing(0.1f), vertexData(),
      restDensity(1.f), stiffness(.8f), viscosity(.5f) // TODO experiment with these arbitrary values
{
    for (size_t i = 0; i < xCount; i++)
    {
        for (size_t j = 0; j < yCount; j++)
        {
            Particle part(
                glm::vec2(i * spacing, j * spacing),
                restDensity,
                restDensity * spacing * spacing);
            particles.push_back(part);
        }
    }
}

ParticleSet::~ParticleSet()
{
}

const std::vector<GLfloat> &ParticleSet::ToVertexData()
{
    vertexData.clear();
    for (std::vector<Particle>::const_iterator it = this->particles.begin(); it != this->particles.end(); ++it)
    {
        // Position
        vertexData.push_back(it->position.x);
        vertexData.push_back(it->position.y);
        // Color
        vertexData.push_back(it->position.x);
        vertexData.push_back(it->position.y);
        vertexData.push_back(0.f);
        vertexData.push_back(1.f);
    }
    return vertexData;
}

void ParticleSet::PrintAllPositions()
{
    for (auto &&particle : this->particles)
    {
        std::cout << particle.position.x << " " << particle.position.y << std::endl;
    }
}
