#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include "Particles.hpp"

std::vector<std::vector<const Particle *> *> *FindAllNeighbors(const ParticleSet &particleSet, const float kernel_support);
float KernelFunction(const glm::vec2 &position_i, const glm::vec2 &position_j, const float &h);
glm::vec2 KernelDerivative(const glm::vec2 &position_i, const glm::vec2 &position_j, const float &h);
