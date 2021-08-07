#pragma once

// Project headers
#include "Experiment.hpp"
#include "Graphics.hpp"
#include "ParticleSet.hpp"
#include "ParticleSimulation.hpp"
#include "Model.hpp"
#include "ParticleSetModel.hpp"
#include "HistoryTracker.hpp"
// Third-party libraries
#include "imgui/imgui.h"           // ImGui::, for displaying user controls in a graphical frame
#include "imgui/implot.h"          // ImPlot::, for plots within ImGui frames
#include <glm/vec2.hpp>            // glm::, for vector maths
#include <glm/gtx/string_cast.hpp> // for casting glm:: objects to string (debug)
// Standard C++ libraries
#include <vector>

// Experiment where a fluid body and some boundaries are simulated.
class BoundaryExperiment : public Experiment
{
public:
    BoundaryExperiment();
    const std::vector<Model *> &models();
    // Starts simulation and visualization.
    void Run();
    // CALLBACKS
    void OnInit();
    // Updates the particle sets for 1 render step
    void OnUpdate();
    // Defines the floating widgets of the GUI
    void OnRender();
    void OnClose();

private:
    // Setup fluid body and boundaries
    void InitializeSimulation(int countX, int countY, float spacing, float restDensity, float stiffness, float viscosity, float boundaryViscosity);
    // Initialize a graphical model for each particle set
    void InitializeModels();

private:
    // Initial properties of the particle sets
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
    // Simulation history
    HistoryTracker historyTracker;
    // Visualization entities
    Graphics graphics;
    std::vector<Model *> _models;
};