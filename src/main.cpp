/**
 * main
 * 
 * Tom Dalling's OpenGL tutorial (http://tomdalling.com/) chapter 1 was taken as starting point for this file.
 */

// third-party libraries

// standard C++ libraries
#include <iostream>
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
    // Init particle set
    ParticleSet particleSet(10, 10, .1f);

    // std::vector<std::vector<const Particle *> *> *allNeighbors = FindAllNeighbors(particleSet, 2 * particleSet.particleSpacing + 1e-6);

    // for (auto &&it = particleSet.particles.begin(); it != particleSet.particles.end(); ++it)
    // {
    //     std::vector<const Particle *> *neighbors = allNeighbors->at(it - particleSet.particles.begin());
    //     glm::vec2 kernelSum(0, 0);
    //     for (auto &&neighbor : *neighbors)
    //     {
    //         kernelSum += KernelDerivative(it->position, neighbor->position, particleSet.particleSpacing);
    //     }
    //     std::cout << std::setw(6) << (glm::length(kernelSum) < .001f) << " ";
    //     if (((it - particleSet.particles.begin() + 1) % 10) == 0)
    //     {
    //         std::cout << std::endl;
    //     }
    // }

    // // Find neighbors for each particle position
    // std::vector<std::vector<const Particle *> *> *allNeighbors = FindAllNeighbors(particleSet, 2 * particleSet.particleSpacing);
    // //
    // for (auto &&it = allNeighbors->begin(); it != allNeighbors->end(); ++it)
    // {
    //     std::cout << std::setw(4) << (*it)->size() << " ";
    //     if (((it - allNeighbors->begin() + 1) % 10) == 0)
    //     {
    //         std::cout << std::endl;
    //     }
    // }
    // std::cout << std::endl;
    // Find neighbors for one particular position
    // Particle &someParticle = particleSet.particles[6 * 10 + 3];
    // std::cout << "My position: " << glm::to_string(someParticle.position) << std::endl;
    // std::vector<const Particle *> *neighbors = (*allNeighbors)[6 * 10 + 3];
    // for (auto &&it = neighbors->begin(); it != neighbors->end(); ++it)
    // {
    //     std::cout << "Neighbor " << it - neighbors->begin() + 1 << ": " << glm::to_string((*it)->position) << std::endl;
    // }

    // create buffer and fill it with the points of the cube
    // LoadVertexData(particleSet);
    // *fluidModel = FluidModelFromParticles(particleSet);

    std::vector<GLfloat> *axesVertexData = new std::vector<GLfloat>{
        -10.f,
        0.f,
        1,
        0,
        0,
        1,
        10.f,
        0.f,
        1,
        0,
        0,
        1,
        0.f,
        -10.f,
        0,
        1,
        0,
        1,
        0.f,
        10.f,
        0,
        1,
        0,
        1,
    };

    Model *axesModel = new Model(*axesVertexData,
                                 ROOT_DIR "resources/graph-shaders/vertex-shader.glsl",
                                 "",
                                 ROOT_DIR "resources/graph-shaders/fragment-shader.glsl",
                                 GL_LINES);
    gGraphics->models.push_back(axesModel);

    gModel = new Model(*particleSet.ToVertexData(),
                       ROOT_DIR "resources/particle-shaders/vertex-shader.glsl",
                       ROOT_DIR "resources/particle-shaders/geometry-shader.glsl",
                       ROOT_DIR "resources/particle-shaders/fragment-shader.glsl",
                       GL_POINTS);

    std::vector<GLfloat> *kernelFunctionGraph1 = KernelFunctionVertexData(1.f);
    Model *kernelModel1 = new Model(*kernelFunctionGraph1,
                                    ROOT_DIR "resources/graph-shaders/vertex-shader.glsl",
                                    "",
                                    ROOT_DIR "resources/graph-shaders/fragment-shader.glsl",
                                    GL_LINE_STRIP);
    gGraphics->models.push_back(kernelModel1);

    std::vector<GLfloat> *kernelFunctionGraph2 = KernelFunctionVertexData(2.f);
    Model *kernelModel2 = new Model(*kernelFunctionGraph2,
                                    ROOT_DIR "resources/graph-shaders/vertex-shader.glsl",
                                    "",
                                    ROOT_DIR "resources/graph-shaders/fragment-shader.glsl",
                                    GL_LINE_STRIP);
    gGraphics->models.push_back(kernelModel2);

    std::vector<GLfloat> *kernelFunctionGraph4 = KernelFunctionVertexData(4.f);
    Model *kernelModel4 = new Model(*kernelFunctionGraph4,
                                    ROOT_DIR "resources/graph-shaders/vertex-shader.glsl",
                                    "",
                                    ROOT_DIR "resources/graph-shaders/fragment-shader.glsl",
                                    GL_LINE_STRIP);
    gGraphics->models.push_back(kernelModel4);

    // std::vector<GLfloat> *kernelDerivativeGraph1 = KernelDerivativeVertexData(1.f);
    // Model *kernelDerivativeModel1 = new Model(*kernelDerivativeGraph1,
    //                                           ROOT_DIR "resources/graph-shaders/vertex-shader.glsl",
    //                                           "",
    //                                           ROOT_DIR "resources/graph-shaders/fragment-shader.glsl",
    //                                           GL_LINE_STRIP);
    // gGraphics->models.push_back(kernelDerivativeModel1);
    // std::vector<GLfloat> *kernelDerivativeGraph2 = KernelDerivativeVertexData(2.f);
    // Model *kernelDerivativeModel2 = new Model(*kernelDerivativeGraph2,
    //                                           ROOT_DIR "resources/graph-shaders/vertex-shader.glsl",
    //                                           "",
    //                                           ROOT_DIR "resources/graph-shaders/fragment-shader.glsl",
    //                                           GL_LINE_STRIP);
    // gGraphics->models.push_back(kernelDerivativeModel2);
    // std::vector<GLfloat> *kernelDerivativeGraph4 = KernelDerivativeVertexData(4.f);
    // Model *kernelDerivativeModel4 = new Model(*kernelDerivativeGraph4,
    //                                           ROOT_DIR "resources/graph-shaders/vertex-shader.glsl",
    //                                           "",
    //                                           ROOT_DIR "resources/graph-shaders/fragment-shader.glsl",
    //                                           GL_LINE_STRIP);
    // gGraphics->models.push_back(kernelDerivativeModel4);

    // gGraphics->models.push_back(gModel);
    std::cout << "Just initialized" << std::endl;
}

void MyOnUpdate()
{
    // std::cout << "Just updated" << std::endl;
}

void MyOnClose()
{
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
