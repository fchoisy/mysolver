#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/vec2.hpp>

class Particle
{
private:
    /* data */
public:
    Particle(/* args */);
    ~Particle();
    glm::vec2 position, velocity;
};

class ParticleSet
{
private:
    /* data */
public:
    ParticleSet(/* args */);
    ParticleSet(int xCount, int yCount, float particleSpacing);
    ~ParticleSet();
    std::vector<GLfloat> *ToVertexData();
    // std::vector<const Particle *> *FindNeighbors(Particle &particle);
    void PrintAllPositions();
    std::vector<Particle> particles;
    GLfloat particleSpacing;
};
