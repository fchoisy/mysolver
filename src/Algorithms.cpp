#include "Algorithms.hpp"

std::vector<std::vector<const Particle *> *> *FindAllNeighbors(const ParticleSet &particleSet)
{
    std::vector<std::vector<const Particle *> *> *allNeighbors = new std::vector<std::vector<const Particle *> *>();
    for (std::vector<Particle>::const_iterator it = particleSet.particles.begin(); it != particleSet.particles.end(); ++it)
    {
        allNeighbors->push_back(FindNeighbors(particleSet, it->position));
    }
    return allNeighbors;
}

std::vector<const Particle *> *FindNeighbors(const ParticleSet &particleSet, const glm::vec2 &paticlePosition)
{
    const float searchRadius = 2 * particleSet.particleSpacing - .00001f;
    std::vector<const Particle *> *neighbors = new std::vector<const Particle *>();
    for (std::vector<Particle>::const_iterator it_neigh = particleSet.particles.begin(); it_neigh != particleSet.particles.end(); ++it_neigh)
    {
        if (glm::distance(paticlePosition, (*it_neigh).position) < searchRadius)
        {
            neighbors->push_back(it_neigh.base());
        }
    }
    return neighbors;
}

/*
Either I do:
FindNeighbors(particleSet)
or
FindNeighbors(particleSet, position)

But then I will loose the benefit of doing it in a triangle.

Should I be dealing with particle identifiers (keys of particles in the container) instead?
What are the other neighbor algorithms like?

*/
