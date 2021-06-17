#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/vec2.hpp>

class Particle
{
private:
    // std::vector<const Particle *> neighbors;

public:
    Particle(const glm::vec2 &position, const GLfloat &density, const GLfloat &mass);
    ~Particle();
    // const std::vector<const Particle *> &GetNeighbors();
    glm::vec2 position, velocity, acceleration;
    GLfloat density, pressure, mass;
};

class ParticleSet
{
private:
    std::vector<GLfloat> vertexData;
    /* data */
public:
    ParticleSet(int xCount, int yCount, GLfloat spacing);
    ~ParticleSet();
    const std::vector<GLfloat> &ToVertexData();
    // std::vector<const Particle *> *FindNeighbors(Particle &particle);
    void PrintAllPositions();
    std::vector<Particle> particles;
    GLfloat spacing, restDensity, stiffness, viscosity;
};
