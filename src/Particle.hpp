#pragma once

#include <glm/vec2.hpp> // glm::vec2

class ParticleSet; // Forward declaration for mutual dependency

class Particle
{
private:
    float volume_, mass_; // Immutable
public:
    Particle(const ParticleSet *set, const glm::vec2 &position, const float &density, const float &volume);
    ~Particle();
    const ParticleSet *set;
    glm::vec2 position, velocity, acceleration, pressureAcceleration, viscosityAcceleration, otherAccelerations;
    float density, pressure;
    float mass() const;
    float volume() const;
};