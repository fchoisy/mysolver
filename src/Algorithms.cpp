#include "Algorithms.hpp"
#include <glm/vec2.hpp>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>
#include <limits>
#include <exception>

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

float KernelFunction(const glm::vec2 &position_i, const glm::vec2 &position_j, const float &support)
{
    if (support < 0)
        throw std::invalid_argument("support must be >= 0");
    // float alpha = 1.f / (6.f * support);  // 1D
    float alpha = 5.f / (14.f * glm::pi<float>() * support * support); // 2D
    // float alpha = 1.f / (4.f * glm::pi<float>() * support * support * support);  // 3D
    float q = glm::distance(position_i, position_j) / support;
    float t1 = glm::max(1.f - q, 0.f);
    float t2 = glm::max(2.f - q, 0.f);
    return alpha * (t2 * t2 * t2 - t1 * t1 * t1);
}
