// Represents a set of particles.
// For example, a fluid body or a boundary.

#pragma once

#include <vector>       // std::vector
#include "Particle.hpp" // Particle

class ParticleSet
{
public:
    // Constructors and destructor
    ParticleSet(int xCount, int yCount, float spacing);
    ParticleSet(int xCount, int yCount, float spacing, float restDensity, float stiffness, float viscosity);
    ~ParticleSet();
    // Methods
    void TranslateAll(float offsetX, float offsetY);
    void PrintAllPositions();
    // Public fields
    std::vector<Particle> particles;
    float spacing, restDensity, stiffness, viscosity;
    bool isBoundary;

private:
    void InitGrid(int xCount, int yCount, float spacing);
};
