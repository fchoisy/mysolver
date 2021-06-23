#pragma once

#include <GL/glew.h>    // GLfloat
#include <vector>       // std::vector
#include <glm/vec2.hpp> // glm::vec2

#include "Particle.hpp" // Particle

class ParticleSet
{
private:
    std::vector<GLfloat> vertexData;
    void InitGrid(int xCount, int yCount, GLfloat spacing);

public:
    ParticleSet(int xCount, int yCount, GLfloat spacing);
    ParticleSet(int xCount, int yCount, GLfloat spacing, GLfloat restDensity, GLfloat stiffness, GLfloat viscosity);
    ~ParticleSet();
    const std::vector<GLfloat> &ToVertexData();
    void PrintAllPositions();
    std::vector<Particle> particles;
    GLfloat spacing, restDensity, stiffness, viscosity;
    bool isStatic;
};
