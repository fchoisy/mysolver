
#include <iostream>
#include "Particles.hpp"

Particle::Particle(/* args */)
{
}

Particle::~Particle()
{
}

ParticleSet::ParticleSet(/* args */)
{
}

ParticleSet::~ParticleSet()
{
}

std::vector<GLfloat> *ParticleSet::ToVertexData()
{
    std::vector<GLfloat> *vertexData = new std::vector<GLfloat>();
    for (std::vector<Particle>::const_iterator it = this->particles.begin(); it != this->particles.end(); ++it)
    {
        // Position
        vertexData->push_back(it->position.x);
        vertexData->push_back(it->position.y);
        // Color
        vertexData->push_back(it->position.x);
        vertexData->push_back(it->position.y);
        vertexData->push_back(0.f);
        vertexData->push_back(1.f);
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


