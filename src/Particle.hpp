
#pragma once

#include <glm/vec2.hpp> // glm::vec2

class ParticleSet; // Forward declaration for mutual dependency

// Represents one single particle and its physical properties.
class Particle
{
public:
    Particle(const ParticleSet *set, const glm::vec2 &position, const float &density, const float &volume);
    ~Particle();
    float mass() const;
    float volume() const;
    const ParticleSet *set; // Set that contains this particle.
    glm::vec2 position, velocity, acceleration, pressureAcceleration, viscosityAcceleration, otherAccelerations;
    float density, pressure;

private:
    float volume_, mass_; // Immutable
};