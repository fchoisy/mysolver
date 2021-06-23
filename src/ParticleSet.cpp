
#include <iostream>
#include "ParticleSet.hpp"

void ParticleSet::InitGrid(int xCount, int yCount, GLfloat spacing)
{
    particles.clear();
    particles.reserve(xCount * yCount);
    for (size_t i = 0; i < xCount; i++)
    {
        for (size_t j = 0; j < yCount; j++)
        {
            Particle part(
                glm::vec2(i * spacing, j * spacing),
                restDensity,
                restDensity * spacing * spacing);
            particles.push_back(part);
        }
    }
}

ParticleSet::ParticleSet(int xCount, int yCount, GLfloat spacing)
    : particles(), vertexData(), spacing(spacing),
      restDensity(1.2f), stiffness(.5f), viscosity(12.f),
      isStatic(false)
{
    InitGrid(xCount, yCount, spacing);
}
ParticleSet::ParticleSet(int xCount, int yCount, GLfloat spacing, GLfloat restDensity, GLfloat stiffness, GLfloat viscosity)
    : particles(), vertexData(), spacing(spacing),
      restDensity(restDensity), stiffness(stiffness), viscosity(viscosity),
      isStatic(false)
{
    InitGrid(xCount, yCount, spacing);
}

ParticleSet::~ParticleSet()
{
}

const std::vector<GLfloat> &ParticleSet::ToVertexData()
{
    vertexData.clear();
    for (std::vector<Particle>::const_iterator it = this->particles.begin(); it != this->particles.end(); ++it)
    {
        // Position
        vertexData.push_back(it->position.x);
        vertexData.push_back(it->position.y);
        // Color
        vertexData.push_back(it->position.x);
        vertexData.push_back(it->position.y);
        vertexData.push_back(0.f);
        vertexData.push_back(1.f);
    }
    return vertexData;
}

void ParticleSet::PrintAllPositions()
{
    for (auto &&particle : this->particles)
    {
        std::cout << particle.position.x << " " << particle.position.y << std::endl;
    }
}
