#include "catch_amalgamated.hpp"

#include <Algorithms.hpp>
#include <Particles.hpp>

TEST_CASE("Number of neighbors", "[neighbors]")
{
    // Generate particle set
    ParticleSet particleSet;
    particleSet.particles = std::vector<Particle>();
    particleSet.particleSpacing = 0.1f;
    for (size_t i = 0; i < 10; i++)
    {
        for (size_t j = 0; j < 10; j++)
        {
            Particle part;
            part.position = glm::vec2(i * particleSet.particleSpacing, j * particleSet.particleSpacing);
            particleSet.particles.push_back(part);
        }
    }
    // Find neighbors
    std::vector<std::vector<const Particle *> *> *allNeighbors = FindAllNeighbors(particleSet);
    // Check number of neighbors
    std::vector<size_t> neighborsCount;
    for (auto &&it = allNeighbors->begin(); it != allNeighbors->end(); ++it)
    {
        neighborsCount.push_back((*it)->size());
    }
    std::vector<size_t> expectedNeighborsCount{
        4,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        4,
        6,
        9,
        9,
        9,
        9,
        9,
        9,
        9,
        9,
        6,
        6,
        9,
        9,
        9,
        9,
        9,
        9,
        9,
        9,
        6,
        6,
        9,
        9,
        9,
        9,
        9,
        9,
        9,
        9,
        6,
        6,
        9,
        9,
        9,
        9,
        9,
        9,
        9,
        9,
        6,
        6,
        9,
        9,
        9,
        9,
        9,
        9,
        9,
        9,
        6,
        6,
        9,
        9,
        9,
        9,
        9,
        9,
        9,
        9,
        6,
        6,
        9,
        9,
        9,
        9,
        9,
        9,
        9,
        9,
        6,
        6,
        9,
        9,
        9,
        9,
        9,
        9,
        9,
        9,
        6,
        4,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        4,
    };
    REQUIRE(neighborsCount == expectedNeighborsCount);
}