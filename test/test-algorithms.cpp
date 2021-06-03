#include "catch_amalgamated.hpp"

#include <iostream>
#include <cstdlib> // Random
#include <ctime>   // To fix seed
#include <cmath>   // For cos and sin
// Tested files
#include <Algorithms.hpp>
#include <Particles.hpp>
#include "test-algorithms.hpp"

void RequireNeighborCountIsCorrect(const ParticleSet &particleSet)
{
    // Find neighbors
    // The search radius is increased slightly to avoid irregularities at the border
    std::vector<std::vector<const Particle *> *> *allNeighborsSmall = FindAllNeighbors(particleSet, 2 * particleSet.particleSpacing + 1.e-05);
    // Check number of neighbors
    std::vector<size_t> neighborsCount;
    for (auto &&it = allNeighborsSmall->begin(); it != allNeighborsSmall->end(); ++it)
    {
        neighborsCount.push_back((*it)->size());
        delete *it;
    }
    delete allNeighborsSmall;
    REQUIRE(neighborsCount == expectedNeighborsCount);
}

void ApplyRandomTranslation(ParticleSet &particleSet)
{

    // Apply random translation to the particle set
    float rangeWidth = 10.f;
    float randomTranslationX = (((float)random()) / (float)(RAND_MAX)) * rangeWidth - .5f * rangeWidth;
    float randomTranslationY = (((float)random()) / (float)(RAND_MAX)) * rangeWidth - .5f * rangeWidth;
    for (auto &&particle : particleSet.particles)
    {
        particle.position.x += randomTranslationX;
        particle.position.y += randomTranslationY;
    }
    RequireNeighborCountIsCorrect(particleSet);
}

void ApplyRandomRotation(ParticleSet &particleSet)
{
    // Apply random rotation to the particle set
    float theta = ((float)rand()) / (float)(RAND_MAX)*3.15f;
    float cosTheta = cos(theta);
    float sinTheta = sin(theta);
    for (auto &&particle : particleSet.particles)
    {
        float oldX = particle.position.x;
        particle.position.x = oldX * cosTheta - particle.position.y * sinTheta;
        particle.position.y = oldX * sinTheta + particle.position.y * cosTheta;
    }
}

TEST_CASE("Number of neighbors is correct", "[neighbors]")
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
    SECTION("before any transformation")
    {
        RequireNeighborCountIsCorrect(particleSet);
    }

    // Set seed for random generator
    srand(time(0));

    // For each SECTION, the TEST_CASE is executed from the start (see Catch2 docs).
    SECTION("after translation")
    {
        ApplyRandomTranslation(particleSet);
        RequireNeighborCountIsCorrect(particleSet);
    }

    SECTION("after rotation")
    {
        ApplyRandomRotation(particleSet);
        RequireNeighborCountIsCorrect(particleSet);
    }
}