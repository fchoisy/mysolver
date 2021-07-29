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

// class HistoryTracker
// {
// private:
//     std::vector<float> timeHistory;
//     std::vector<std::vector<Particle>> targetHistory;
//     const ParticleSet *target;

// public:
//     HistoryTracker()
//         : target(nullptr)
//     {
//     }

//     void SetTarget(const ParticleSet *target)
//     {
//         this->target = target;
//     }

//     void Step(float currentTime)
//     {
//         if (target != nullptr)
//         {
//             timeHistory.push_back(currentTime);
//             targetHistory.push_back(target->particles);
//         }
//     }

//     std::vector<float> &GetTimeHistory()
//     {
//         return timeHistory;
//     }

//     std::vector<std::vector<Particle>> &GetTargetHistory()
//     {
//         return targetHistory;
//     }

//     void Clear()
//     {
//         timeHistory.clear();
//         targetHistory.clear();
//     }
// };

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
    float timeStep; // should respect the Courant-Friedrich-Levy condition
    const glm::vec2 gravity;
    // Simulation entities
    // ParticleSet particleSet;
    // ParticleSet boundary;
    // ParticleSet boundary2;
    // ParticleSet boundary3;
    std::vector<ParticleSet> particleSets;
    ParticleSimulation particleSimulation;
    // Visualization entities
    Graphics graphics;
    std::vector<Model *> _models;
    // Simulation history
    HistoryTracker historyTracker;
    // std::vector<float> timeHistory;
    // std::vector<std::vector<Particle>> particleSetHistory;

    void InitializeSimulation(int countX, int countY, float spacing, float restDensity, float stiffness, float viscosity, float boundaryViscosity)
    {
        // Initialize particle sets
        particleSets.clear();
        ParticleSet fluid = ParticleSet(countX, countY, spacing, restDensity, stiffness, viscosity);
        ParticleSet boundary = ParticleSet(30, 3, spacing, restDensity, stiffness, viscosity);
        ParticleSet boundary2 = ParticleSet(3, 30, spacing, restDensity, stiffness, viscosity);
        ParticleSet boundary3 = ParticleSet(3, 30, spacing, restDensity, stiffness, viscosity);
        // fluid.TranslateAll(0.f, 3.f * spacing);
        // fluid.TranslateAll(1.f * spacing, 0.f);
        boundary.TranslateAll(-10.f * spacing, -3.f * spacing);
        boundary2.TranslateAll(-3.f * spacing, 0.f * spacing);
        boundary3.TranslateAll(10.f * spacing, 0.f * spacing);
        boundary.isStatic = true;
        boundary2.isStatic = true;
        boundary3.isStatic = true;
        particleSets.push_back(fluid);
        particleSets.push_back(boundary);
        particleSets.push_back(boundary2);
        particleSets.push_back(boundary3);
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
        // _models.push_back(new ParticleSetModel(particleSet));
        // _models.push_back(new ParticleSetModel(boundary));
        // _models.push_back(new ParticleSetModel(boundary2));
        // _models.push_back(new ParticleSetModel(boundary3));
        for (auto &&model : _models)
        {
            model->Update();
        }
    }

public:
    BoundaryExperiment()
        : defaultCountX(1), defaultCountY(1),
          defaultSpacing(3.f),
          defaultRestDensity(3e3f),
          defaultStiffness(3e7f),
          defaultViscosity(2e-7f),
          defaultBoundaryViscosity(4e-2),
          currentTime(0.f),
          timeStep(.01f),
          gravity(0.f, -9.81f),
          //   particleSet(defaultCountX, defaultCountY, defaultSpacing, defaultRestDensity, defaultStiffness, defaultViscosity),
          //   boundary(30, 3, defaultSpacing, defaultRestDensity, defaultStiffness, defaultBoundaryViscosity),
          //   boundary2(3, 30, defaultSpacing, defaultRestDensity, defaultStiffness, defaultBoundaryViscosity),
          //   boundary3(3, 30, defaultSpacing, defaultRestDensity, defaultStiffness, defaultBoundaryViscosity),
          graphics(*this)
    {
        InitializeSimulation(defaultCountX, defaultCountY, defaultSpacing, defaultRestDensity, defaultStiffness, defaultViscosity, defaultBoundaryViscosity);
        // particleSimulation.AddParticleSet(particleSet);
        // particleSimulation.AddParticleSet(boundary);
        // for (auto &&ps : particleSets)
        // {
        //     particleSimulation.AddParticleSet(ps);
        // }
        // particleSimulation.AddParticleSet(boundary2);
        // particleSimulation.AddParticleSet(boundary3);
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
        // _models.clear();
        // for (auto &&ps : particleSets)
        // {
        //     _models.push_back(new ParticleSetModel(ps));
        // }
        // // _models.push_back(new ParticleSetModel(particleSet));
        // // _models.push_back(new ParticleSetModel(boundary));
        // // _models.push_back(new ParticleSetModel(boundary2));
        // // _models.push_back(new ParticleSetModel(boundary3));
        // for (auto &&model : _models)
        // {
        //     model->Update();
        // }
    }

    void OnUpdate()
    {
        for (int i = 0; i < 5; i++)
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
            // timeHistory.push_back(currentTime);
            // particleSetHistory.push_back(particleSet.particles);
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
        ImGui::Begin("Simulation Parameters", NULL, ImGuiWindowFlags_AlwaysAutoResize);
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
                for (auto &&ps : historyTracker.GetTargetHistory())
                {
                    velocityHistory.push_back(glm::length(ps.at(0).velocity));
                    pressureAccelerationHistory.push_back(glm::length(ps.at(0).pressureAcceleration) / 10.f);
                    viscosityAccelerationHistory.push_back(glm::length(ps.at(0).viscosityAcceleration) / 10.f);
                    otherAccelerationsHistory.push_back(glm::length(ps.at(0).otherAccelerations) / 10.f);
                    pressureHistory.push_back(ps.at(0).pressure / 10000.f);
                    densityHistory.push_back(ps.at(0).density / 1000.f);
                }
                ImPlot::PlotLine("velocity", historyTracker.GetTimeHistory().data(), velocityHistory.data(), velocityHistory.size());
                // ImPlot::PlotLine("pressureAcceleration", historyTracker.GetTimeHistory().data(), pressureAccelerationHistory.data(), pressureAccelerationHistory.size());
                // ImPlot::PlotLine("viscosityAcceleration", historyTracker.GetTimeHistory().data(), viscosityAccelerationHistory.data(), viscosityAccelerationHistory.size());
                // ImPlot::PlotLine("otherAccelerations", historyTracker.GetTimeHistory().data(), otherAccelerationsHistory.data(), otherAccelerationsHistory.size());
                ImPlot::PlotLine("pressure", historyTracker.GetTimeHistory().data(), pressureHistory.data(), pressureHistory.size());
                // ImPlot::PlotLine("density", historyTracker.GetTimeHistory().data(), densityHistory.data(), densityHistory.size());
                ImPlot::EndPlot();
            }
        }
        if (ImGui::CollapsingHeader("Reset simulation", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("Number of particles");
            static int newNoParticlesX = 3;
            ImGui::SliderInt("x", &newNoParticlesX, 1, 10);

            static int newNoParticlesY = 4;
            ImGui::SliderInt("y", &newNoParticlesY, 1, 10);

            static float newRestDensity = defaultRestDensity;
            ImGui::InputFloat("Rest density", &newRestDensity, 0.0F, 0.0F, "%e");

            static float newStiffness = defaultStiffness;
            ImGui::InputFloat("Stiffness", &newStiffness, 0.0F, 0.0F, "%e");

            static float newViscosity = defaultViscosity;
            ImGui::InputFloat("Viscosity", &newViscosity, 0.0F, 0.0F, "%e");

            if (ImGui::Button("Reset"))
            {
                historyTracker.Clear();
                InitializeSimulation(newNoParticlesX, newNoParticlesY, defaultSpacing, newRestDensity, newStiffness, newViscosity, defaultBoundaryViscosity);
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
