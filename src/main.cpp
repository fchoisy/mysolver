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
    Graphics gGraphics;
    Model *gModel = NULL;
    Model *gModelBoundary = NULL;
    ParticleSet particleSet;
    ParticleSet boundary;
    ParticleSimulation particleSimulation;
    Kernel kernel;
    // Simulation parameters
    GLfloat currentTime;
    const GLfloat timeStep; // should respect the Courant-Friedrich-Levy condition
    const glm::vec2 gravity;
    // Simulation history
    std::vector<float> timeHistory;
    std::vector<std::vector<Particle>> particleSetHistory;

public:
    BoundaryExperiment()
        : gravity(0.f, -9.f),
          particleSet(1, 1, .1f, 3e3f, 3e5f, 1e-7f),
          boundary(90, 3, .1f, 3e3f, 3000.f, 1e-7f),
          timeStep(.01f),
          currentTime(0.f),
          kernel(particleSet.spacing),
          gGraphics(*this)
    {
        particleSimulation.AddParticleSet(particleSet);
        boundary.isStatic = true;
        boundary.TranslateAll(-4.f, -.4f);
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

        timeHistory.push_back(currentTime);
        particleSetHistory.push_back(particleSet.particles);
        // std::cout << "Just updated" << std::endl;
    }

    void OnRender()
    {
        ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
        ImGui::Begin("Dashboard", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        if (ImGui::CollapsingHeader("Status", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("t = %03f", currentTime);

            if (ImPlot::BeginPlot("Velocity of particle 0", "t", "v(t)"))
            {
                std::vector<float> velocityHistory;
                for (auto &&ps : particleSetHistory)
                {
                    velocityHistory.push_back(glm::length(ps.at(0).velocity));
                }

                ImPlot::PlotLine("velocity", timeHistory.data(), velocityHistory.data(), velocityHistory.size());
                ImPlot::EndPlot();
            }
        }
        if (ImGui::CollapsingHeader("Reset simulation", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("Number of particles");
            ImGui::SameLine();
            static int newNoParticlesX = 1;
            ImGui::SliderInt("x", &newNoParticlesX, 1, 10);

            static int newNoParticlesY = 1;
            ImGui::SameLine();
            ImGui::SliderInt("y", &newNoParticlesY, 1, 10);

            ImGui::Text("RestDensity");
            ImGui::SameLine();
            static float newRestDensityMantissa = 3.0f;
            static const float mantissaMin = 0.f;
            static const float mantissaMax = 10.f;
            ImGui::SliderScalar("rho *10^", ImGuiDataType_Float, &newRestDensityMantissa, &mantissaMin, &mantissaMax);
            ImGui::SameLine();
            static int newRestDensityExponent = 3;
            ImGui::InputInt("RestDensity exponent", &newRestDensityExponent, 1);

            ImGui::Text("Stiffness");
            ImGui::SameLine();
            static float newStiffnessMantissa = 3.0f;
            static const float stiffnessMantissaMin = 0.f;
            static const float stiffnessMantissaMax = 10.f;
            ImGui::SliderScalar("k *10^", ImGuiDataType_Float, &newStiffnessMantissa, &stiffnessMantissaMin, &stiffnessMantissaMax);
            ImGui::SameLine();
            static int newStiffnessExponent = 5;
            ImGui::InputInt("stiffness exponent", &newStiffnessExponent, 1);

            ImGui::Text("Viscosity");
            ImGui::SameLine();
            static float newViscosityMantissa = 2.0f;
            static const float viscosityMantissaMin = 0.f;
            static const float viscosityMantissaMax = 10.f;
            ImGui::SliderScalar("nu *10^", ImGuiDataType_Float, &newViscosityMantissa, &viscosityMantissaMin, &viscosityMantissaMax);
            ImGui::SameLine();
            static int newViscosityExponent = -7;
            ImGui::InputInt("Viscosity exponent", &newViscosityExponent, 1);

            // static float newRestDensity = particleSet.restDensity;
            // ImGui::InputFloat("Rest density", &newRestDensity, 0.0F, 0.0F, "%e");

            // static float newViscosity = particleSet.viscosity;
            // ImGui::InputFloat("Viscosity", &newViscosity, 0.0F, 0.0F, "%e");

            if (ImGui::Button("Reset"))
            {
                float newRestDensity = newRestDensityMantissa * pow(10, newRestDensityExponent);
                float newStiffness = newStiffnessMantissa * pow(10, newStiffnessExponent);
                float newViscosity = newViscosityMantissa * pow(10, newViscosityExponent);

                particleSet = ParticleSet(newNoParticlesX, newNoParticlesY, .1f, newRestDensity, newStiffness, newViscosity);
                currentTime = 0.f;
                timeHistory.clear();
                particleSetHistory.clear();
            }
        }
        // ImGui::ShowDemoWindow();
        // ImPlot::ShowDemoWindow();
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
