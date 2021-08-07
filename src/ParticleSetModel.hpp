// Graphical representation of a set of particles.

#pragma once

#include "Model.hpp"
#include "ParticleSet.hpp"
// #include <glm/geometric.hpp>  // glm::length

class ParticleSetModel : public Model
{

public:
    ParticleSetModel(const ParticleSet &particleSet);
    // Converts the data of the particle set to vertex data for rendering
    void Update();

private:
    void UpdateVertexData();
    const ParticleSet &particleSet;
    std::vector<GLfloat> vertexData;
};