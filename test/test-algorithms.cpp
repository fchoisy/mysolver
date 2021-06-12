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

using namespace Catch;

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
    const float h = .1f;
    const float frac = 1.f / (14.f * glm::pi<float>() * h * h);
    const float t = glm::pow((2.f - glm::sqrt(2.f)), 3.f);

    SECTION("for distance = 0, h and h * sqrt(2)")
    {
        const glm::vec2 reference(2.f, 7.f);
        REQUIRE(Approx(20.f * frac) == KernelFunction(reference, reference, h));
        REQUIRE(Approx(5.f * frac) == KernelFunction(reference, reference + glm::vec2(h, 0.f), h));
        REQUIRE(Approx(5 * t * frac) == KernelFunction(reference, reference + glm::vec2(h, h), h));
        std::cout << "Kernel function special values" << std::endl;
        std::cout << "\tW(0) = " << KernelFunction(reference, reference, h) << std::endl;
        std::cout << "\tW(0.1) = " << KernelFunction(reference, reference + glm::vec2(h, 0.f), h) << std::endl;
        std::cout << "\tW(0.1 * sqrt(2)) = " << KernelFunction(reference, reference + glm::vec2(h, h), h) << std::endl;
    }

    SECTION("sum for particles in a 5x5 grid using neighbor search")
    {
        const float expected_sum_in_corner = (30.f + 5.f * t) * frac;
        const float expected_sum_on_edge = (35.f + 10.f * t) * frac;
        const float expected_sum_when_surrounded = (40.f + 20.f * t) * frac;

        // Generate particle set
        ParticleSet particleSet(5, 5, h);
        std::vector<std::vector<const Particle *> *> *allNeighbors = FindAllNeighbors(particleSet, 2 * particleSet.particleSpacing + 1e-6);

        std::cout << "Kernel function sum" << std::endl;
        // for (auto &&it = particleSet.particles.begin(); it != particleSet.particles.end(); ++it)
        for (size_t i = 0; i < particleSet.particles.size(); ++i)
        {
            float kernelSum(0.f);
            for (auto &&neighbor : *((*allNeighbors)[i]))
            {
                kernelSum += KernelFunction(particleSet.particles[i].position, neighbor->position, particleSet.particleSpacing);
            }

            bool is_top_or_bottom = (i < 5 || i > 19);
            bool is_left_or_right = (i % 5 == 0 || i % 5 == 4);

            if (is_top_or_bottom && is_left_or_right) // is in corner
            {
                REQUIRE(Approx(expected_sum_in_corner) == kernelSum);
            }
            else if (is_top_or_bottom || is_left_or_right) // is on edge
            {
                REQUIRE(Approx(expected_sum_on_edge) == kernelSum);
            }
            else // is surrounded by particles on all sides
            {
                REQUIRE(Approx(expected_sum_when_surrounded) == kernelSum);
            }

            std::cout << std::right << std::setprecision(3) << std::setw(10) << kernelSum << " ";
            if (((i + 1) % 5) == 0)
            {
                std::cout << std::endl;
            }
        }
    }
}

TEST_CASE("Kernel derivative", "[kernel]")
{
    // Generate particle set
    ParticleSet particleSet(5, 5, .1f);
    std::vector<std::vector<const Particle *> *> *allNeighbors = FindAllNeighbors(particleSet, 2 * particleSet.particleSpacing + 1e-6);

    std::cout << "Kernel derivative sum" << std::endl;
    for (auto &&it = particleSet.particles.begin(); it != particleSet.particles.end(); ++it)
    {
        std::vector<const Particle *> *neighbors = allNeighbors->at(it - particleSet.particles.begin());
        glm::vec2 kernelSum(0, 0);
        for (auto &&neighbor : *neighbors)
        {
            kernelSum += KernelFunctionDerivative(it->position, neighbor->position, particleSet.particleSpacing);
        }
        float lengthToPrint = glm::length(kernelSum);
        std::cout << std::right << std::setprecision(3) << std::setw(10) << lengthToPrint << " ";
        if (((it - particleSet.particles.begin() + 1) % 5) == 0)
        {
            std::cout << std::endl;
        }
    }
}