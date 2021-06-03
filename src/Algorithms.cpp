#include "Algorithms.hpp"
#include <glm/gtc/matrix_transform.hpp> // Vector maths
#include <glm/gtc/epsilon.hpp>
#include <limits>

std::vector<std::vector<const Particle *> *> *FindAllNeighbors(const ParticleSet &particle_set, const float kernel_support)
{
    std::vector<std::vector<const Particle *> *> *all_neighbors = new std::vector<std::vector<const Particle *> *>();
    for (auto &&particle : particle_set.particles)
    {
        std::vector<const Particle *> *neighbors = new std::vector<const Particle *>();
        for (auto &other_particle : particle_set.particles)
        {
            if (glm::distance(particle.position, other_particle.position) < kernel_support)
            {
                neighbors->push_back(&other_particle);
            }
        }
        all_neighbors->push_back(neighbors);
    }
    return all_neighbors;
}
