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
    bool lButtonDown;
    glm::vec3 cameraOffset;
    double oldX;
    double oldY;
};

class Graphics
{
private:
    Experiment &experiment;
    const glm::vec2 SCREEN_SIZE;
    GLFWwindow *gWindow;
    InternalState internalState;
    static void OnError(int errorCode, const char *msg);
    static void OnKey(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void OnMouseButton(GLFWwindow *window, int button, int action, int mods);
    static void OnCursorPos(GLFWwindow *window, double newX, double newY);
    static void OnScroll(GLFWwindow *window, double xoffset, double yoffset);

public:
    Graphics(Experiment &experiment);
    ~Graphics();
    void Update();
    void Render();
    void Run();
};
