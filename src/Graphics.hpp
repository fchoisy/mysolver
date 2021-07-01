#pragma once

#include <vector>
#include <GLFW/glfw3.h>
#include "Model.hpp"
#include "Experiment.hpp"

struct InternalState
{
    float zoomLevel;
    bool isPaused;
    bool shouldUpdateOneStep;
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
    static void OnError(int errorCode, const char *msg);
    static void OnScroll(GLFWwindow *window, double xoffset, double yoffset);
    static void OnKey(GLFWwindow *window, int key, int scancode, int action, int mods);

public:
    Graphics(Experiment &experiment);
    ~Graphics();
    void Update();
    void Render();
    void Run();
    std::vector<const Model *> models;
};
