/**
 * main
 * 
 * Entry point to the program.
 * Contains the callbacks that are called by the GUI.
 */

// third-party libraries

// standard C++ libraries
#include <iostream>
#include <iomanip> // std::setw()
#include <limits>
#include <vector>

#include <GL/glew.h>               // Runtime loading of OpenGL API functions
#include <GLFW/glfw3.h>            // Windowing
#include <glm/vec2.hpp>            // Vector maths
#include <glm/gtx/string_cast.hpp> // For debugging
#include <tdogl/Program.h>

#include "imgui/imgui.h"

#include "Graphics.hpp"
#include "ParticleSet.hpp"
#include "ParticleSimulation.hpp"
#include "Model.hpp"
#include "Kernel.hpp"
#include "helpers/RootDir.h"

Model *gModel = NULL;
Graphics *gGraphics = NULL;
std::vector<GLfloat> gPosition;
GLfloat currentTime = 0.f;

static const glm::vec2 gravity(0.f, -9.f);

// Init particle set
ParticleSet particleSet(10, 10, .1f, 1.2f, .3f, .8f);
// ParticleSet particleSet(1, 1, .1f);
ParticleSimulation particleSimulation;

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

    // Model *axesModel = Model::Axes();
    // gGraphics->models.push_back(axesModel);

    particleSimulation.AddParticleSet(particleSet);

    gModel = Model::ParticleVisualization(particleSet.ToVertexData());
    gGraphics->models.push_back(gModel);

    // gGraphics->models.push_back(gModel);
    std::cout << "Just initialized" << std::endl;
}

void MyOnUpdate()
{
    particleSimulation.UpdateNeighbors(2 * particleSet.spacing - 1.e-05);
    particleSimulation.UpdateParticles(timeStep, gravity);

    gModel->SetVertexData(particleSet.ToVertexData());
    currentTime += timeStep;

    gPosition.push_back(particleSet.particles[0].position.y);
    std::cout << "Just updated" << std::endl;
}

void MyOnRender()
{
    ImGui::Begin("Status");
    ImGui::Text("t = %03f", currentTime);

    // const GLfloat my_values[] = {0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f};
    ImGui::PlotLines("Positions", gPosition.data(), gPosition.size());
    // ImGui::PlotLines("Positions", gPosition.data(), sizeof(my_values));
    ImGui::End();
    ;
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
    gGraphics = new Graphics(&MyOnInit, &MyOnUpdate, &MyOnRender, &MyOnClose);
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
