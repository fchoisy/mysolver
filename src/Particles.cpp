
#include <iostream>
#include "Particles.hpp"

Particle::Particle(/* args */)
    : position(0.f, 0.f), velocity(0.f, 0.f), acceleration(0.f, 0.f),
      density(1.f), pressure(1.f), mass(1.f)
{
}

Particle::~Particle()
{
}

ParticleSet::ParticleSet(int xCount, int yCount, float spacing)
    : particles(), spacing(0.1f), vertexData(),
      restDensity(1.f), stiffness(.8f), viscosity(.5f)
{
    for (size_t i = 0; i < xCount; i++)
    {
        for (size_t j = 0; j < yCount; j++)
        {
            Particle part;
            part.position = glm::vec2(i * spacing, j * spacing);
            part.density = restDensity;
            part.mass = restDensity * spacing * spacing;
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
