#pragma once

#include <vector>
#include <GLFW/glfw3.h>
#include "Model.hpp"
#include "Experiment.hpp"

struct InternalState
{
    float zoomLevel;
    // float cameraX;
    // float cameraY;
};

class Graphics
{
private:
    Experiment &experiment;
    const glm::vec2 SCREEN_SIZE;
    GLFWwindow *gWindow;
    InternalState internalState;

public:
    Graphics(Experiment &experiment);
    ~Graphics();
    static void OnError(int errorCode, const char *msg);
    void Update();
    void Render();
    void Run();
    std::vector<const Model *> models;
};
