

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

std::vector<const Particle *> *ParticleSet::FindNeighbors(Particle &particle)
{
    const float searchRadius = 2 * this->particleSpacing - .00001f;
    std::vector<const Particle *> *neighbors = new std::vector<const Particle *>();
    for (std::vector<Particle>::const_iterator it_neigh = this->particles.begin(); it_neigh != this->particles.end(); ++it_neigh)
    {
        if (&particle != it_neigh.base() && glm::distance(particle.position, (*it_neigh).position) < searchRadius)
        {
            neighbors->push_back(it_neigh.base());
        }
    }
    return neighbors;
}
