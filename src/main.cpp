/**
 * main
 * 
 * Entry point to the program.
 * Contains the callbacks that are called by the GUI.
 */

// Project headers
#include "Experiment.hpp"
#include "Graphics.hpp"
#include "ParticleSet.hpp"
#include "ParticleSimulation.hpp"
#include "Model.hpp"
#include "ParticleSetModel.hpp"
#include "HistoryTracker.hpp"
#include "helpers/RootDir.h"
// Third-party libraries
#include "imgui/imgui.h"           // ImGUI for simulation parameters
#include "imgui/implot.h"          // Betters plots for ImGUI
#include <GL/glew.h>               // Runtime loading of OpenGL API functions
#include <GLFW/glfw3.h>            // Windowing
#include <glm/vec2.hpp>            // Vector maths
#include <glm/gtx/string_cast.hpp> // For debugging
// Standard C++ libraries
#include <iostream>
#include <iomanip> // std::setw()
#include <limits>
#include <vector>

class BoundaryExperiment : public Experiment
{
private:
    const int defaultCountX;
    const int defaultCountY;
    const float defaultSpacing;
    const float defaultRestDensity;
    const float defaultStiffness;
    const float defaultViscosity;
    const float defaultBoundaryViscosity;
    // Simulation parameters
    float currentTime;
    float timeStep;
    int simulationStepsPerRender;
    const glm::vec2 gravity;
    // Simulation entities
    std::vector<ParticleSet> particleSets;
    ParticleSimulation particleSimulation;
    // Visualization entities
    Graphics graphics;
    std::vector<Model *> _models;
    // Simulation history
    HistoryTracker historyTracker;

    void InitializeSimulation(int countX, int countY, float spacing, float restDensity, float stiffness, float viscosity, float boundaryViscosity)
    {
        // Initialize particle sets:
        particleSets.clear();

        // - Fluid
        particleSets.push_back(ParticleSet(countX, countY, spacing, restDensity, stiffness, viscosity));

        // - Boundaries
        particleSets.push_back(ParticleSet(26, 3, spacing, restDensity, stiffness, boundaryViscosity));
        particleSets.back().TranslateAll(-3.f * spacing, -3.f * spacing);
        particleSets.back().isBoundary = true;

        particleSets.push_back(ParticleSet(3, 20, spacing, restDensity, stiffness, boundaryViscosity));
        particleSets.back().TranslateAll(-3.f * spacing, 0.f * spacing);
        particleSets.back().isBoundary = true;

        particleSets.push_back(ParticleSet(3, 20, spacing, restDensity, stiffness, boundaryViscosity));
        particleSets.back().TranslateAll(20.f * spacing, 0.f * spacing);
        particleSets.back().isBoundary = true;

        // particleSets.push_back(ParticleSet(11, 3, spacing, restDensity, stiffness, boundaryViscosity));
        // particleSets.back().TranslateAll(-3.f * spacing, 5.f * spacing);
        // particleSets.back().isBoundary = true;

        // Bind history tracker to the particle fluid
        historyTracker.SetTarget(&particleSets.front());

        // Add particle sets to simulation
        particleSimulation.Clear();
        for (auto &&ps : particleSets)
        {
            particleSimulation.AddParticleSet(ps);
        }
    }

    void InitializeModels()
    {
        _models.clear();
        for (auto &&ps : particleSets)
        {
            _models.push_back(new ParticleSetModel(ps));
        }
        for (auto &&model : _models)
        {
            model->Update();
        }
    }

public:
    BoundaryExperiment()
        : defaultCountX(10), defaultCountY(10),
          defaultSpacing(3.f),
          defaultRestDensity(3e3f),
          defaultStiffness(4e7f),
          defaultViscosity(2e-7f),
          defaultBoundaryViscosity(4e-2),
          currentTime(0.f),
          timeStep(.01f),
          simulationStepsPerRender(5),
          gravity(0.f, -9.81f),
          graphics(*this)
    {
        InitializeSimulation(defaultCountX, defaultCountY, defaultSpacing, defaultRestDensity, defaultStiffness, defaultViscosity, defaultBoundaryViscosity);
    }

    const std::vector<Model *> &models()
    {
        return _models;
    }

    void Run()
    {
        graphics.Run();
    }

    void OnInit()
    {
        InitializeModels();
    }

    void OnUpdate()
    {
        for (int i = 0; i < simulationStepsPerRender; i++)
        {
            // Simulation step
            particleSimulation.UpdateNeighbors(2 * defaultSpacing);
            // particleSimulation.UpdateParticles(timeStep, gravity);
            // timeStep = particleSimulation.UpdateParticles(0.f, gravity);
            particleSimulation.UpdateParticleQuantities(gravity);
            // timeStep = particleSimulation.ComputeTimeStep(0.4);
            particleSimulation.UpdateParticlePositions(timeStep);
            currentTime += timeStep;
            // Record history (for plotting)
            historyTracker.Step(currentTime);
        }
        // Update models (for visualization)
        for (auto &&model : _models)
        {
            model->Update();
        }
    }

    // Defines the floating widgets of the Graphical User Interface
    void OnRender()
    {
        // ImPlot::ShowDemoWindow();
        ImGui::Begin("Simulation Parameters");
        if (ImGui::CollapsingHeader("User Guide"))
        {
            ImPlot::ShowUserGuide();
            ImGui::BulletText("Press Space to Play/Pause the simulation visualization.");
            ImGui::BulletText("Press Enter to render 1 step.");
            ImGui::BulletText("Press Escape to quit.");
        }
        if (ImGui::CollapsingHeader("Time, size, distance", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("t = %f", currentTime);
            ImGui::SameLine();
            ImGui::InputFloat("Time step", &timeStep, 0.f, 0.f, "%f");
            ImGui::SliderInt("Simulation steps per render step", &simulationStepsPerRender, 1, 20);
            ImGui::Text("h = %f", defaultSpacing);
            // ImGui::Text("Time step = %f", timeStep);
            if (ImPlot::BeginPlot("Maximum distance traveled by a particle", "time", "magnitude", ImVec2(-1, 0), 0, 0, ImPlotAxisFlags_AutoFit))
            {
                ImPlot::PlotLine("Maximum distance", historyTracker.GetTimeHistory().data(), historyTracker.maxDistance.data(), historyTracker.maxDistance.size());
                float particleSize[2] = {defaultSpacing, defaultSpacing};
                float time[2] = {0.f, 0.f};
                if (historyTracker.GetTimeHistory().size() >= 1)
                {
                    time[0] = historyTracker.GetTimeHistory().front();
                    time[1] = historyTracker.GetTimeHistory().back();
                }
                ImPlot::PlotLine("Particle size", time, particleSize, 2);

                ImPlot::EndPlot();
            }
        }
        if (ImGui::CollapsingHeader("Particle Quantities", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImPlot::BeginPlot("Properties of the particle of index 0", "time", "magnitude", ImVec2(-1, 0), 0, 0, ImPlotAxisFlags_AutoFit))
            {
                ImPlot::SetLegendLocation(ImPlotLocation_South, ImPlotOrientation_Vertical, true);
                ImPlot::PlotLine("Density", historyTracker.GetTimeHistory().data(), historyTracker.density[0].data(), historyTracker.density[0].size());
                ImPlot::PlotLine("Pressure", historyTracker.GetTimeHistory().data(), historyTracker.pressure[0].data(), historyTracker.pressure[0].size());
                ImPlot::PlotLine("Pressure acceleration", historyTracker.GetTimeHistory().data(), historyTracker.pressureAcceleration[0].data(), historyTracker.pressureAcceleration[0].size());
                ImPlot::PlotLine("Viscosity acceleration", historyTracker.GetTimeHistory().data(), historyTracker.viscosityAcceleration[0].data(), historyTracker.viscosityAcceleration[0].size());
                ImPlot::PlotLine("Other accelerations", historyTracker.GetTimeHistory().data(), historyTracker.otherAccelerations[0].data(), historyTracker.otherAccelerations[0].size());
                ImPlot::PlotLine("Velocity", historyTracker.GetTimeHistory().data(), historyTracker.velocity[0].data(), historyTracker.velocity[0].size());
                ImPlot::EndPlot();
            }
        }
        if (ImGui::CollapsingHeader("Reset simulation", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("Number of particles");
            static int newNoParticlesX = defaultCountX;
            ImGui::SliderInt("x", &newNoParticlesX, 1, 20);

            static int newNoParticlesY = defaultCountY;
            ImGui::SliderInt("y", &newNoParticlesY, 1, 20);

            static float newRestDensity = defaultRestDensity;
            ImGui::InputFloat("Rest density", &newRestDensity, 0.0F, 0.0F, "%e");

            static float newStiffness = defaultStiffness;
            ImGui::InputFloat("Stiffness", &newStiffness, 0.0F, 0.0F, "%e");

            static float newViscosity = defaultViscosity;
            ImGui::InputFloat("Viscosity", &newViscosity, 0.0F, 0.0F, "%e");

            static float newBoundaryViscosity = defaultBoundaryViscosity;
            ImGui::InputFloat("Boundary viscosity", &newBoundaryViscosity, 0.0F, 0.0F, "%e");

            if (ImGui::Button("Reset"))
            {
                historyTracker.Clear();
                InitializeSimulation(newNoParticlesX, newNoParticlesY, defaultSpacing, newRestDensity, newStiffness, newViscosity, newBoundaryViscosity);
                InitializeModels();
                currentTime = 0.f;
            }
        }
        ImGui::End();
    }

    void OnClose()
    {
    }
};

int main(int argc, char *argv[])
{
    try
    {
        BoundaryExperiment boundaryExperiment;
        boundaryExperiment.Run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "ERROR caught at highest level: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
