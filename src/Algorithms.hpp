#pragma once

#include <vector>
#include "Particles.hpp"

std::vector<std::vector<const Particle *> *> *FindAllNeighbors(const ParticleSet &particleSet, const float kernel_support);