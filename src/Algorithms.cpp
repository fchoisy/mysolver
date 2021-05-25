#include "Algorithms.hpp"

std::vector<const Particle *> *FindNeighbors(ParticleSet &particleSet, Particle &particle)
{
    const float searchRadius = 2 * particleSet.particleSpacing - .00001f;
    std::vector<const Particle *> *neighbors = new std::vector<const Particle *>();
    for (std::vector<Particle>::const_iterator it_neigh = particleSet.particles.begin(); it_neigh != particleSet.particles.end(); ++it_neigh)
    {
        if (&particle != it_neigh.base() && glm::distance(particle.position, (*it_neigh).position) < searchRadius)
        {
            neighbors->push_back(it_neigh.base());
        }
    }
    return neighbors;
}
