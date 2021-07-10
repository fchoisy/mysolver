#pragma once

#include "helpers/RootDir.h"
#include "Model.hpp"

class ParticleSetModel : public Model
{
private:
    const ParticleSet &particleSet;
    std::vector<GLfloat> vertexData;

    void UpdateVertexData()
    {
        vertexData.clear();
        for (auto &&particle : particleSet.particles)
        {
            // Position
            vertexData.push_back(particle.position.x);
            vertexData.push_back(particle.position.y);
            // Color
            vertexData.push_back(particle.pressure / 1000.f);
            vertexData.push_back(0.f);
            // vertexData.push_back(particle.position.y);
            vertexData.push_back(0.f);
            vertexData.push_back(1.f);
        }
    }

public:
    ParticleSetModel(const ParticleSet &particleSet)
        : Model(ROOT_DIR "resources/particle-shaders/vertex-shader.glsl",
                ROOT_DIR "resources/particle-shaders/geometry-shader.glsl",
                ROOT_DIR "resources/particle-shaders/fragment-shader.glsl",
                GL_POINTS),
          particleSet(particleSet)
    {
    }

    void Update()
    {
        // Get vertex data
        UpdateVertexData();
        // Copy vertex data to GPU
        SetVertexData(vertexData);
    }
};