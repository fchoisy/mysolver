#pragma once

#include <vector>
#include <glm/gtc/matrix_transform.hpp> // Vector maths
#include "Particles.hpp"

int dummy(int input);

std::vector<const Particle *> *FindNeighbors(const ParticleSet &particleSet, const glm::vec2 &paticlePosition);
std::vector<std::vector<const Particle *> *> *FindAllNeighbors(const ParticleSet &particleSet);