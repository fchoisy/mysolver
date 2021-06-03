#include "catch_amalgamated.hpp"

#include <iostream>
#include <cstdlib>                 // Random
#include <ctime>                   // To fix seed
#include <cmath>                   // For cos and sin
#include <glm/geometric.hpp>       // Vector maths
#include <glm/gtx/string_cast.hpp> // For debugging
#include <iomanip>                 // std::setw()
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
    ParticleSet particleSet(10, 10, .1f);

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

TEST_CASE("Kernel function", "[kernel]")
{
    // Generate particle set
    ParticleSet particleSet(10, 10, .1f);
}

TEST_CASE("Kernel derivative", "[kernel]")
{
    // Generate particle set
    ParticleSet particleSet(10, 10, .1f);
    std::vector<std::vector<const Particle *> *> *allNeighbors = FindAllNeighbors(particleSet, 2 * particleSet.particleSpacing + 1e-6);

    for (auto &&it = particleSet.particles.begin(); it != particleSet.particles.end(); ++it)
    {
        std::vector<const Particle *> *neighbors = allNeighbors->at(it - particleSet.particles.begin());
        glm::vec2 kernelSum(0, 0);
        for (auto &&neighbor : *neighbors)
        {
            kernelSum += KernelFunctionDerivative(it->position, neighbor->position, particleSet.particleSpacing);
        }
        std::cout << std::setw(6) << (glm::length(kernelSum) < .001f) << " ";
        if (((it - particleSet.particles.begin() + 1) % 10) == 0)
        {
            std::cout << std::endl;
        }
    }
}