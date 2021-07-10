#pragma once

#include <GL/glew.h>    // GLfloat
#include <glm/vec2.hpp> // glm::vec2

class Particle
{
private:
public:
    Particle(const glm::vec2 &position, const GLfloat &density, const GLfloat &mass);
    ~Particle();
    glm::vec2 position, velocity, acceleration, pressureAcceleration, viscosityAcceleration, otherAccelerations;
    GLfloat density, pressure, mass;
};