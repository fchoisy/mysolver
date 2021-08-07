#pragma once

#include "Experiment.hpp"
#include <vector>
#include <GLFW/glfw3.h>

// Data that can be modified by (static) event listeners.
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

// Class for rendering the different models of an Experiment using the graphics library.
// Also handles play/pause functionality of the simulation.
class Graphics
{
public:
    Graphics(Experiment &experiment);
    ~Graphics();
    // Initializes and starts the render loop.
    void Run();
    // Updates the experiment, if not paused.
    void Update();
    // Render all the models in the experiment.
    void Render();

private:
    // Callback for OpenGL errors
    static void OnError(int errorCode, const char *msg);
    // Event listeners
    static void OnKey(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void OnMouseButton(GLFWwindow *window, int button, int action, int mods);
    static void OnCursorPos(GLFWwindow *window, double newX, double newY);
    static void OnScroll(GLFWwindow *window, double xoffset, double yoffset);
    static const float ZOOM_SPEED;
    static const float ZOOM_MIN;
    static const float ZOOM_MAX;
    // Non-static private fields
    const glm::vec2 SCREEN_SIZE;
    Experiment &experiment;
    GLFWwindow *gWindow;
    InternalState internalState;
};
