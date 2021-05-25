#pragma once

#include <vector>
#include "Particles.hpp"

std::vector<const Particle *> *FindNeighbors(ParticleSet &particleSet, Particle &particle);