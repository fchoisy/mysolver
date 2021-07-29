#include "ParticleSetModel.hpp"

#include "helpers/RootDir.h" // ROOT_DIR

ParticleSetModel::ParticleSetModel(const ParticleSet &particleSet)
    : Model(ROOT_DIR "resources/particle-shaders/vertex-shader.glsl",
            ROOT_DIR "resources/particle-shaders/geometry-shader.glsl",
            ROOT_DIR "resources/particle-shaders/fragment-shader.glsl",
            GL_POINTS),
      particleSet(particleSet)
{
}

void ParticleSetModel::Update()
{
    // Get vertex data
    UpdateVertexData();
    // Copy vertex data to GPU
    SetVertexData(vertexData);
}

void ParticleSetModel::UpdateVertexData()
{
    vertexData.clear();
    for (auto &&particle : particleSet.particles)
    {
        // Position
        vertexData.push_back(particle.position.x);
        vertexData.push_back(particle.position.y);
        // Color
        vertexData.push_back(particle.pressure / 1000.f);
        // vertexData.push_back(glm::length(particle.velocity));
        vertexData.push_back(0.f);
        // vertexData.push_back(particle.position.y);
        vertexData.push_back(0.f);
        vertexData.push_back(1.f);
    }
}