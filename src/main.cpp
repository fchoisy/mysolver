/**
 * main
 * 
 * Tom Dalling's OpenGL tutorial (http://tomdalling.com/) chapter 1 was taken as starting point for this file.
 */

// third-party libraries

// standard C++ libraries
#include <iostream>
#include <fstream>
#include <iomanip> // std::setw()
#include <limits>

#include <GL/glew.h>               // Runtime loading of OpenGL API functions
#include <GLFW/glfw3.h>            // Windowing
#include <glm/vec2.hpp>            // Vector maths
#include <glm/gtx/string_cast.hpp> // For debugging
#include <tdogl/Program.h>

#include "Graphics.hpp"
#include "Particles.hpp"
#include "Model.hpp"
#include "Algorithms.hpp"
#include "helpers/RootDir.h"

Model *gModel = NULL;
Graphics *gGraphics = NULL;

// std::ofstream gOutputFile;

static const glm::vec2 gravity(0.f, -9.f);

// Init particle set
ParticleSet particleSet(10, 10, .1f);
// ParticleSet particleSet(1, 1, .1f);

static const GLfloat timeStep = .01f; // should respect the Courant-Friedrich-Levy condition

std::vector<GLfloat> *KernelFunctionVertexData(const float &h)
{
    std::vector<GLfloat> *kernelFunctionGraph = new std::vector<GLfloat>;
    glm::vec2 origin(0, 0);
    const uint numSteps = 100;
    const float rangeStart = -8.f;
    const float rangeStop = 8.f;
    const float stepSize = (rangeStop - rangeStart) / numSteps;
    for (uint i = 0; i < numSteps; i++)
    {
        float x = rangeStart + stepSize * i;
        float y = KernelFunction(origin, glm::vec2(x, 0.f), h) * 10.f;
        // std::cout << y << std::endl;
        kernelFunctionGraph->push_back(x);
        kernelFunctionGraph->push_back(y);
        kernelFunctionGraph->push_back(1);
        kernelFunctionGraph->push_back(1);
        kernelFunctionGraph->push_back(1);
        kernelFunctionGraph->push_back(1);
    }
    return kernelFunctionGraph;
}

std::vector<GLfloat> *KernelDerivativeVertexData(const float &h)
{
    std::vector<GLfloat> *kernelDerivativeGraph = new std::vector<GLfloat>;
    glm::vec2 origin(0, 0);
    const uint numSteps = 100;
    const float rangeStart = -8.f;
    const float rangeStop = 8.f;
    const float stepSize = (rangeStop - rangeStart) / numSteps;
    for (uint i = 0; i < numSteps; i++)
    {
        float x = rangeStart + stepSize * i;
        float y = KernelDerivative(origin, glm::vec2(x, 0.f), h).x * 10.f;
        kernelDerivativeGraph->push_back(x);
        kernelDerivativeGraph->push_back(y);
        kernelDerivativeGraph->push_back(1);
        kernelDerivativeGraph->push_back(1);
        kernelDerivativeGraph->push_back(1);
        kernelDerivativeGraph->push_back(1);
    }
    return kernelDerivativeGraph;
}

void MyOnInit()
{
    // gOutputFile.open("output.txt");

    // Model *axesModel = Model::Axes();
    // gGraphics->models.push_back(axesModel);

    // gModel = new Model(*particleSet.ToVertexData(),
    //                    ROOT_DIR "resources/particle-shaders/vertex-shader.glsl",
    //                    ROOT_DIR "resources/particle-shaders/geometry-shader.glsl",
    //                    ROOT_DIR "resources/particle-shaders/fragment-shader.glsl",
    //                    GL_POINTS);
    gModel = Model::ParticleVisualization(particleSet.ToVertexData());
    gGraphics->models.push_back(gModel);

    // std::vector<GLfloat> *kernelFunctionGraph1 = KernelFunctionVertexData(1.f);
    // Model *kernelModel1 = Model::Graph(*kernelFunctionGraph1);
    // gGraphics->models.push_back(kernelModel1);

    // std::vector<GLfloat> *kernelFunctionGraph2 = KernelFunctionVertexData(2.f);
    // Model *kernelModel2 = Model::Graph(*kernelFunctionGraph2);
    // gGraphics->models.push_back(kernelModel2);

    // std::vector<GLfloat> *kernelFunctionGraph4 = KernelFunctionVertexData(4.f);
    // Model *kernelModel4 = Model::Graph(*kernelFunctionGraph4);
    // gGraphics->models.push_back(kernelModel4);

    // gGraphics->models.push_back(gModel);
    std::cout << "Just initialized" << std::endl;
}

void MyOnUpdate()
{
    std::vector<std::vector<const Particle *> *> *allNeighbors = FindAllNeighbors(particleSet, 2 * particleSet.spacing - 1.e-05);
    for (size_t i = 0; i < particleSet.particles.size(); ++i)
    {
        Particle &particle = particleSet.particles[i];
        particle.density = 0.f;
        for (auto &&neighbor : *((*allNeighbors)[i]))
        {
            particle.density += neighbor->mass * KernelFunction(particle.position, neighbor->position, particleSet.spacing);
        }
        particle.pressure = glm::max(particleSet.stiffness * (particle.density / particleSet.restDensity - 1.f), 0.f);
    }

    for (size_t i = 0; i < particleSet.particles.size(); ++i)
    {
        Particle &particle = particleSet.particles[i];
        glm::vec2 viscosityAcceleration(0.f, 0.f);
        for (auto &&neighbor : *((*allNeighbors)[i]))
        {
            glm::vec2 positionDiff = neighbor->position - particle.position;
            glm::vec2 velocityDiff = neighbor->velocity - particle.velocity;
            viscosityAcceleration += (neighbor->mass / neighbor->density) * (velocityDiff * positionDiff) / (glm::dot(positionDiff, positionDiff) + 0.01f * particleSet.spacing * particleSet.spacing) * KernelDerivative(particle.position, neighbor->position, particleSet.spacing);
        }
        viscosityAcceleration *= 2.f; // dimensionality
        viscosityAcceleration *= particleSet.viscosity;

        glm::vec2 pressureAcceleration(0.f, 0.f);
        for (auto &&neighbor : *((*allNeighbors)[i]))
        {
            pressureAcceleration += neighbor->mass * (particle.pressure / (particle.density * particle.density) + neighbor->pressure / (neighbor->density * neighbor->density)) * KernelDerivative(particle.position, neighbor->position, particleSet.spacing);
        }
        pressureAcceleration *= -1.f;

        particle.acceleration = viscosityAcceleration + gravity + pressureAcceleration;
    }

    for (size_t i = 0; i < particleSet.particles.size(); ++i)
    {
        Particle &particle = particleSet.particles[i];
        particle.velocity += timeStep * particle.acceleration;
        particle.position += timeStep * particle.velocity;
    }

    for (size_t i = 0; i < particleSet.particles.size(); ++i)
    {
        Particle &particle = particleSet.particles[i];
        // std::cout << particle.position.x << " " << particle.position.y << std::endl;
        std::cout << particle.density << " " << particle.pressure << std::endl;
    }
    gModel->SetVertexData(particleSet.ToVertexData());
    std::cout << "Just updated" << std::endl;
}

void MyOnClose()
{
    // gOutputFile.close();
    if (gModel)
        delete gModel;
    std::cout << "Just closed" << std::endl;
}

// the program starts here
void AppMain()
{
    // void (*OnInit)() = &MyOnInit;
    // void (*OnUpdate)() = &MyOnUpdate;
    // void (*OnClose)() = &MyOnClose;
    gGraphics = new Graphics(&MyOnInit, &MyOnUpdate, &MyOnClose);
    gGraphics->Run();
    delete gGraphics;
}

int main(int argc, char *argv[])
{
    try
    {
        AppMain();
    }
    catch (const std::exception &e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
