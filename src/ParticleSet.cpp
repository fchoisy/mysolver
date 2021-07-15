
#include "ParticleSet.hpp"

#include <iostream>     // std::cout
#include <glm/vec2.hpp> // glm::vec2

void ParticleSet::InitGrid(int xCount, int yCount, float spacing)
{
    const float volume = spacing * spacing;
    particles.clear();
    particles.reserve(xCount * yCount);
    for (size_t i = 0; i < xCount; i++)
    {
        for (size_t j = 0; j < yCount; j++)
        {
            Particle part(
                this,
                glm::vec2(i * spacing, j * spacing),
                restDensity,
                volume);
            particles.push_back(part);
        }
    }
}

ParticleSet::ParticleSet(int xCount, int yCount, float spacing)
    : particles(), spacing(spacing),
      restDensity(1.2f), stiffness(.5f), viscosity(12.f),
      isStatic(false)
{
    InitGrid(xCount, yCount, spacing);
}
ParticleSet::ParticleSet(int xCount, int yCount, float spacing, float restDensity, float stiffness, float viscosity)
    : particles(), spacing(spacing),
      restDensity(restDensity), stiffness(stiffness), viscosity(viscosity),
      isStatic(false)
{
    InitGrid(xCount, yCount, spacing);
}

ParticleSet::~ParticleSet()
{
}

void ParticleSet::TranslateAll(float offsetX, float offsetY)
{
    for (auto &&particle : particles)
    {
        particle.position.x += offsetX;
        particle.position.y += offsetY;
    }
}

void ParticleSet::PrintAllPositions()
{
    for (auto &&particle : this->particles)
    {
        std::cout << particle.position.x << " " << particle.position.y << std::endl;
    }
}
