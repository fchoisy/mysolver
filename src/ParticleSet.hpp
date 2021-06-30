#pragma once

#include <GL/glew.h>    // GLfloat
#include <vector>       // std::vector
#include <glm/vec2.hpp> // glm::vec2

#include "Particle.hpp" // Particle

class ParticleSet
{
private:
    void InitGrid(int xCount, int yCount, GLfloat spacing);
    std::vector<GLfloat> vertexData;

public:
    // Constructors and destructor
    ParticleSet(int xCount, int yCount, GLfloat spacing);
    ParticleSet(int xCount, int yCount, GLfloat spacing, GLfloat restDensity, GLfloat stiffness, GLfloat viscosity);
    ~ParticleSet();
    // Methods
    void TranslateAll(GLfloat offsetX, GLfloat offsetY);
    const std::vector<GLfloat> &ToVertexData();
    void PrintAllPositions();
    // Public fields
    std::vector<Particle> particles;
    GLfloat spacing, restDensity, stiffness, viscosity;
    bool isStatic;
};
