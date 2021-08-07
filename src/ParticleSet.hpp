
#pragma once

#include "Particle.hpp" // Particle
#include <vector>       // std::vector

// Represents a set of particles.
// For example, a fluid body or a boundary.
class ParticleSet
{
public:
    ParticleSet(int xCount, int yCount, float spacing, float restDensity, float stiffness, float viscosity);
    ~ParticleSet();
    // Shift all particle positions by a horizontal and a vertical offset.
    void TranslateAll(float offsetX, float offsetY);
    // Print out all particle positions.
    void PrintAllPositions();
    std::vector<Particle> particles;
    // Properties that are uniform accross particles of the same body.
    float spacing, restDensity, stiffness, viscosity;
    bool isBoundary;

private:
    // Fill the set with particles whose positions form a regular grid.
    void InitGrid(int xCount, int yCount, float spacing);
};
