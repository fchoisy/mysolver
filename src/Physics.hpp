#pragma once

#include <array>

#include <glm/gtc/matrix_transform.hpp> // Vector maths

struct Particle
{
    glm::vec2 position, velocity;
    std::vector<const Particle *> neighbors;
};

struct ParticleSet
{
    std::vector<Particle> particles;
    GLfloat particleSpacing;
};