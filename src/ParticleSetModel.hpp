#pragma once

#include "Model.hpp"
#include "ParticleSet.hpp"
// #include <glm/geometric.hpp>  // glm::length

class ParticleSetModel : public Model
{
private:
    const ParticleSet &particleSet;
    std::vector<GLfloat> vertexData;
    void UpdateVertexData();

public:
    ParticleSetModel(const ParticleSet &particleSet);
    void Update();
};