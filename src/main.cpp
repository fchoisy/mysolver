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

#include "imgui/imgui.h"
#include "imgui/implot.h"

#include "Experiment.hpp"
#include "Graphics.hpp"
#include "ParticleSet.hpp"
#include "ParticleSimulation.hpp"
#include "Model.hpp"
#include "ParticleSetModel.hpp"
#include "Kernel.hpp"
#include "helpers/RootDir.h"

class BoundaryExperiment : public Experiment
{
private:
    const int defaultCountX;
    const int defaultCountY;
    const float spacing;
    const float defaultRestDensity;
    const float defaultStiffness;
    const float defaultViscosity;
    const float defaultBoundaryViscosity;
    // Simulation parameters
    float currentTime;
    float timeStep; // should respect the Courant-Friedrich-Levy condition
    const glm::vec2 gravity;
    // Simulation entities
    ParticleSet particleSet;
    ParticleSet boundary;
    ParticleSet boundary2;
    ParticleSet boundary3;
    ParticleSimulation particleSimulation;
    Kernel kernel;
    // Visualization entities
    Graphics graphics;
    std::vector<Model *> _models;
    // Simulation history
    std::vector<float> timeHistory;
    std::vector<std::vector<Particle>> particleSetHistory;

    void InitializeSimulation(int countX, int countY, float spacing, float restDensity, float stiffness, float viscosity, float boundaryViscosity)
    {
        particleSet = ParticleSet(countX, countY, spacing, restDensity, stiffness, viscosity);
        boundary = ParticleSet(30, 3, spacing, restDensity, stiffness, viscosity);
        boundary2 = ParticleSet(3, 30, spacing, restDensity, stiffness, viscosity);
        boundary3 = ParticleSet(3, 30, spacing, restDensity, stiffness, viscosity);
        // particleSet.TranslateAll(0.f, 3.f * spacing);
        // particleSet.TranslateAll(1.f * spacing, 0.f);
        boundary.TranslateAll(-10.f * spacing, -3.f * spacing);
        boundary2.TranslateAll(-3.f * spacing, 0.f * spacing);
        boundary3.TranslateAll(3.f * spacing, 0.f * spacing);
        boundary.isStatic = true;
        boundary2.isStatic = true;
        boundary3.isStatic = true;
    }

public:
    BoundaryExperiment()
        : defaultCountX(1), defaultCountY(1),
          spacing(3.f),
          defaultRestDensity(3e3f),
          defaultStiffness(3e7f),
          defaultViscosity(2e-7f),
          defaultBoundaryViscosity(4e-2),
          currentTime(0.f),
          timeStep(.01f),
          gravity(0.f, -9.81f),
          particleSet(defaultCountX, defaultCountY, spacing, defaultRestDensity, defaultStiffness, defaultViscosity),
          boundary(30, 3, spacing, defaultRestDensity, defaultStiffness, defaultBoundaryViscosity),
          boundary2(3, 30, spacing, defaultRestDensity, defaultStiffness, defaultBoundaryViscosity),
          boundary3(3, 30, spacing, defaultRestDensity, defaultStiffness, defaultBoundaryViscosity),
          kernel(spacing),
          graphics(*this)
    {
        InitializeSimulation(defaultCountX, defaultCountY, spacing, defaultRestDensity, defaultStiffness, defaultViscosity, defaultBoundaryViscosity);
        particleSimulation.AddParticleSet(particleSet);
        particleSimulation.AddParticleSet(boundary);
        particleSimulation.AddParticleSet(boundary2);
        particleSimulation.AddParticleSet(boundary3);
        graphics.Run();
    }

    const std::vector<Model *> &models()
    {
        return _models;
    }

    void OnInit()
    {
        _models.push_back(new ParticleSetModel(particleSet));
        _models.push_back(new ParticleSetModel(boundary));
        _models.push_back(new ParticleSetModel(boundary2));
        _models.push_back(new ParticleSetModel(boundary3));
        for (auto &&model : _models)
        {
            model->Update();
        }
    }

    void OnUpdate()
    {
        // Simulation step
        particleSimulation.UpdateNeighbors(2 * particleSet.spacing);
        // particleSimulation.UpdateParticles(timeStep, gravity);
        // timeStep = particleSimulation.UpdateParticles(0.f, gravity);
        particleSimulation.UpdateParticleQuantities(gravity);
        // timeStep = particleSimulation.ComputeTimeStep(0.4);
        particleSimulation.UpdateParticlePositions(timeStep);
        currentTime += timeStep;
        // Record history (for plotting)
        timeHistory.push_back(currentTime);
        particleSetHistory.push_back(particleSet.particles);
        // Update models (for visualization)
        for (auto &&model : _models)
        {
            model->Update();
        }
    }

    // Defines the floating widgets of the Graphical User Interface
    void OnRender()
    {
        ImGui::Begin("Dashboard", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        if (ImGui::CollapsingHeader("Status", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("t = %f", currentTime);
            ImGui::SameLine();
            ImGui::InputFloat("Time step", &timeStep, 0.f, 0.f, "%f");
            // ImGui::Text("Time step = %f", timeStep);

            if (ImPlot::BeginPlot("Properties of particle 0", "t", "magnitude"))
            {
                std::vector<float> velocityHistory;
                std::vector<float> pressureAccelerationHistory;
                std::vector<float> viscosityAccelerationHistory;
                std::vector<float> otherAccelerationsHistory;
                std::vector<float> pressureHistory;
                std::vector<float> densityHistory;
                for (auto &&ps : particleSetHistory)
                {
                    velocityHistory.push_back(glm::length(ps.at(0).velocity));
                    pressureAccelerationHistory.push_back(glm::length(ps.at(0).pressureAcceleration) / 10.f);
                    viscosityAccelerationHistory.push_back(glm::length(ps.at(0).viscosityAcceleration) / 10.f);
                    otherAccelerationsHistory.push_back(glm::length(ps.at(0).otherAccelerations) / 10.f);
                    pressureHistory.push_back(ps.at(0).pressure / 10000.f);
                    densityHistory.push_back(ps.at(0).density / 1000.f);
                }
                ImPlot::PlotLine("velocity", timeHistory.data(), velocityHistory.data(), velocityHistory.size());
                // ImPlot::PlotLine("pressureAcceleration", timeHistory.data(), pressureAccelerationHistory.data(), pressureAccelerationHistory.size());
                // ImPlot::PlotLine("viscosityAcceleration", timeHistory.data(), viscosityAccelerationHistory.data(), viscosityAccelerationHistory.size());
                // ImPlot::PlotLine("otherAccelerations", timeHistory.data(), otherAccelerationsHistory.data(), otherAccelerationsHistory.size());
                ImPlot::PlotLine("pressure", timeHistory.data(), pressureHistory.data(), pressureHistory.size());
                // ImPlot::PlotLine("density", timeHistory.data(), densityHistory.data(), densityHistory.size());
                ImPlot::EndPlot();
            }
        }
        if (ImGui::CollapsingHeader("Reset simulation", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("Number of particles");
            // ImGui::SameLine();
            static int newNoParticlesX = 3;
            ImGui::SliderInt("x", &newNoParticlesX, 1, 10);

            static int newNoParticlesY = 4;
            // ImGui::SameLine();
            ImGui::SliderInt("y", &newNoParticlesY, 1, 10);

            static float newRestDensity = particleSet.restDensity;
            ImGui::InputFloat("Rest density", &newRestDensity, 0.0F, 0.0F, "%e");

            static float newStiffness = particleSet.stiffness;
            ImGui::InputFloat("Stiffness", &newStiffness, 0.0F, 0.0F, "%e");

            static float newViscosity = particleSet.viscosity;
            ImGui::InputFloat("Viscosity", &newViscosity, 0.0F, 0.0F, "%e");

            if (ImGui::Button("Reset"))
            {
                // float newRestDensity = newRestDensityMantissa * pow(10, newRestDensityExponent);
                // float newStiffness = newStiffnessMantissa * pow(10, newStiffnessExponent);
                // float newViscosity = newViscosityMantissa * pow(10, newViscosityExponent);

                InitializeSimulation(newNoParticlesX, newNoParticlesY, spacing, newRestDensity, newStiffness, newViscosity, defaultBoundaryViscosity);
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
    }
};

// class ParameterSearch
// {
// private:
//     ParticleSet boundary;
//     ParticleSimulation particleSimulation;
//     Kernel kernel;

//     float currentTime;
//     const float timeStep; // should respect the Courant-Friedrich-Levy condition
//     const glm::vec2 gravity;

// public:
//     ParameterSearch()
//         : gravity(0.f, -9.81f),
//           boundary(90, 3, .1f, 3e3f, 3000.f, 1e-7f),
//           timeStep(.01f),
//           currentTime(0.f),
//           kernel(particleSet.spacing),
//     {
//     ParticleSet particleSet(1, 1, .1f, 3e3f, 3e5f, 1e-7f);

//     }
// }

int main(int argc, char *argv[])
{
    try
    {
        BoundaryExperiment boundaryExperiment;
    }
    catch (const std::exception &e)
    {
        std::cerr << "ERROR caught at highest level: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
