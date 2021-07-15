#pragma once

#include <vector>       // std::vector
#include "Particle.hpp" // Particle

class ParticleSet
{
private:
    void InitGrid(int xCount, int yCount, float spacing);

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
    bool isStatic;
};
