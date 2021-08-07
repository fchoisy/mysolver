#include "catch_amalgamated.hpp" // Test framework

#include "TestParticleSimulation.hpp"
// Tested files
#include <ParticleSet.hpp>
#include <ParticleSimulation.hpp>
// Libraries
#include <glm/geometric.hpp>       // Vector maths
#include <glm/gtx/string_cast.hpp> // For debugging
#include <iomanip>                 // std::setw()
#include <iostream>                // std::cout
#include <limits>                  // std::numeric_limits
#include <cstdlib>                 // Random
#include <ctime>                   // To fix seed
#include <cmath>                   // For cos and sin

using namespace Catch; // Test framework

static const float epsilon = std::numeric_limits<float>::epsilon() * 100;

void RequireNeighborCountIsCorrect(ParticleSet &particleSet)
{
    // Find neighbors
    ParticleSimulation particleSimulation;
    particleSimulation.AddParticleSet(particleSet);
    // The search radius is decreased slightly to avoid irregularities at the border
    particleSimulation.UpdateNeighbors(2 * particleSet.spacing - 1.e-5f);
    // Check number of neighbors
    std::vector<size_t> neighborsCount;
    for (auto &&particle : particleSet.particles)
    {
        neighborsCount.push_back(particleSimulation.GetNeighbors(particle).size());
    }
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
    ParticleSet particleSet(10, 10, .2f, 0.f, 0.f, 0.f);

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
