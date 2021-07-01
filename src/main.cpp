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
#include "imgui/implot.h"

#include "Experiment.hpp"
#include "Graphics.hpp"
#include "ParticleSet.hpp"
#include "ParticleSimulation.hpp"
#include "Model.hpp"
#include "Kernel.hpp"
#include "helpers/RootDir.h"

class BoundaryExperiment : public Experiment
{
private:
    Model *gModel = NULL;
    Model *gModelBoundary = NULL;
    Graphics gGraphics;
    std::vector<GLfloat> gPosition;
    GLfloat currentTime = 0.f;

    const glm::vec2 gravity;
    // static const glm::vec2 gravity(0.f, 0.f);

    // Init particle set
    ParticleSet particleSet;
    ParticleSet boundary;
    // ParticleSet particleSet(1, 1, .1f);
    ParticleSimulation particleSimulation;

    const GLfloat timeStep; // should respect the Courant-Friedrich-Levy condition

    Kernel kernel;

public:
    BoundaryExperiment()
        : gravity(0.f, -9.f),
          particleSet(1, 1, .1f, 3e3f, 3e5f, 1e-7f),
          boundary(30, 3, .1f, 3e3f, 3000.f, 1e-7f),
          timeStep(.01f),
          kernel(particleSet.spacing),
          gGraphics(*this)
    {
        particleSimulation.AddParticleSet(particleSet);
        boundary.isStatic = true;
        boundary.TranslateAll(-2.f, -.4f);
        particleSimulation.AddParticleSet(boundary);
        gGraphics.Run();
    }

    ~BoundaryExperiment()
    {
        if (gModel)
            delete gModel;
        if (gModelBoundary)
            delete gModelBoundary;
    }

    void OnInit()
    {

        // Model *axesModel = Model::Axes();
        // gGraphics->models.push_back(axesModel);

        gModel = Model::ParticleVisualization(particleSet.ToVertexData());
        gGraphics.models.push_back(gModel);
        gModelBoundary = Model::ParticleVisualization(boundary.ToVertexData());
        gGraphics.models.push_back(gModelBoundary);

        // gGraphics.models.push_back(gModel);
        // std::cout << "Just initialized" << std::endl;
    }

    void OnUpdate()
    {
        particleSimulation.UpdateNeighbors(2 * particleSet.spacing - 1.e-05);
        particleSimulation.UpdateParticles(timeStep, gravity);

        gModel->SetVertexData(particleSet.ToVertexData());
        gModelBoundary->SetVertexData(boundary.ToVertexData());
        currentTime += timeStep;

        gPosition.push_back(particleSet.particles[0].position.y);
        // std::cout << "Just updated" << std::endl;
    }

    void OnRender()
    {
        ImGui::Begin("Status");
        ImGui::Text("t = %03f", currentTime);

        // const GLfloat my_values[] = {0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f};
        // ImGui::PlotLines("Positions", gPosition.data(), gPosition.size());
        // ImGui::PlotLines("Positions", gPosition.data(), sizeof(my_values));

        if (ImGui::TreeNode("Reset simulation"))
        {

            // static int newNoParticlesX = 10;
            // ImGui::SliderInt("Number of particles in the x direction", &newNoParticlesX, 1, 100);

            // static int newNoParticlesY = 10;
            // ImGui::SliderInt("Number of particles in the y direction", &newNoParticlesY, 1, 100);

            static float newStiffness = particleSet.stiffness;
            ImGui::InputFloat("Stiffness", &newStiffness, 0.0F, 0.0F, "%e");

            static float newRestDensity = particleSet.restDensity;
            ImGui::InputFloat("Rest density", &newRestDensity, 0.0F, 0.0F, "%e");

            static float newViscosity = particleSet.viscosity;
            ImGui::InputFloat("Viscosity", &newViscosity, 0.0F, 0.0F, "%e");

            if (ImGui::Button("Reset"))
            {
                // particleSet = ParticleSet(newNoParticlesX, newNoParticlesY, .1f, newRestDensity, newStiffness, newViscosity);
                particleSet = ParticleSet(10, 10, .1f, newRestDensity, newStiffness, newViscosity);
            }
            ImGui::TreePop();
        }
        // ImGui::ShowDemoWindow();
        ImPlot::ShowDemoWindow();
        ImGui::End();
        ;
    }

    void OnClose()
    {
        // std::cout << "Just closed" << std::endl;
    }
};

// the program starts here
void AppMain()
{
    BoundaryExperiment boundaryExperiment;
}

int main(int argc, char *argv[])
{
    try
    {
        AppMain();
    }
    catch (const std::exception &e)
    {
        std::cerr << "ERROR caught at highest level: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
