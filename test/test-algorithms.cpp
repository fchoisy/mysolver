#include "catch_amalgamated.hpp"

#include <iostream>
#include <cstdlib> // Random
#include <ctime>   // To fix seed
#include <cmath>   // For cos and sin
#include <Algorithms.hpp>
#include <Particles.hpp>

void RequireNeighborCountIsCorrect(const ParticleSet &particleSet)
{
    static const std::vector<size_t> expectedNeighborsCount{
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
    // Find neighbors
    std::vector<std::vector<const Particle *> *> *allNeighbors = FindAllNeighbors(particleSet);
    // Check number of neighbors
    std::vector<size_t> neighborsCount;
    for (auto &&it = allNeighbors->begin(); it != allNeighbors->end(); ++it)
    {
        neighborsCount.push_back((*it)->size());
        delete *it;
    }
    delete allNeighbors;
    REQUIRE(neighborsCount == expectedNeighborsCount);
}

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

    RequireNeighborCountIsCorrect(particleSet);

    // TODO change seed!
    srand(time(0));

    // Random uniform translation
    float randomTranslationX = (((float)random()) / RAND_MAX) * 100.f - 50.f;
    float randomTranslationY = (((float)random()) / RAND_MAX) * 100.f - 50.f;
    for (auto &&particle : particleSet.particles)
    {
        particle.position.x += randomTranslationX;
        particle.position.y += randomTranslationY;
    }
    RequireNeighborCountIsCorrect(particleSet);

    // Random uniform rotation
    float theta = ((float)rand()) / RAND_MAX * 3.15f;
    for (auto &&particle : particleSet.particles)
    {
        float oldX = particle.position.x;
        particle.position.x = oldX * cos(theta) - particle.position.y * sin(theta);
        particle.position.y = oldX * sin(theta) + particle.position.y * cos(theta);
    }
    RequireNeighborCountIsCorrect(particleSet);
}