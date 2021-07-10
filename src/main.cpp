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
    Graphics graphics;
    ParticleSet particleSet;
    ParticleSet boundary;
    ParticleSet boundary2;
    ParticleSet boundary3;
    std::vector<Model *> _models;
    ParticleSimulation particleSimulation;
    Kernel kernel;
    // Simulation parameters
    GLfloat currentTime;
    GLfloat timeStep; // should respect the Courant-Friedrich-Levy condition
    const glm::vec2 gravity;
    // Simulation history
    std::vector<float> timeHistory;
    std::vector<std::vector<Particle>> particleSetHistory;

public:
    BoundaryExperiment()
        : graphics(*this),
          particleSet(1, 1, .1f, 3e3f, 3e5f, 1e-7f),
          boundary(90, 3, .1f, 3e3f, 3000.f, 1e-7f),
          boundary2(3, 30, .1f, 3e3f, 3000.f, 1e-7f),
          boundary3(3, 30, .1f, 3e3f, 3000.f, 1e-7f),
          //   particleSetModel(particleSet),
          //   boundaryModel(boundary),
          currentTime(0.f),
          timeStep(.01f),
          gravity(0.f, -9.81f),
          kernel(particleSet.spacing)
    {
        particleSimulation.AddParticleSet(particleSet);
        boundary.isStatic = true;
        boundary.TranslateAll(-4.f, -.3f);
        particleSimulation.AddParticleSet(boundary);
        boundary2.isStatic = true;
        boundary2.TranslateAll(-.3f, 0.f);
        particleSimulation.AddParticleSet(boundary2);
        boundary3.isStatic = true;
        boundary3.TranslateAll(.3f, 0.f);
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
        particleSimulation.UpdateParticles(timeStep, gravity);
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
            ImGui::Text("t = %03f", currentTime);
            ImGui::SameLine();
            ImGui::InputFloat("Time step", &timeStep);

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
    }
};

// class ParameterSearch
// {
// private:
//     ParticleSet boundary;
//     ParticleSimulation particleSimulation;
//     Kernel kernel;

//     GLfloat currentTime;
//     const GLfloat timeStep; // should respect the Courant-Friedrich-Levy condition
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
