#pragma once

#include <vector>
#include <GLFW/glfw3.h>
#include "Model.hpp"

struct InternalState
{
    float zoomLevel;
    // float cameraX;
    // float cameraY;
};

class Graphics
{
private:
    void (*_OnInit)();
    void (*_OnUpdate)();
    void (*_OnClose)();
    const glm::vec2 SCREEN_SIZE;
    GLFWwindow *gWindow;
    InternalState internalState;

public:
    Graphics(void (*OnInit)(), void (*OnUpdate)(), void (*OnClose)());
    ~Graphics();
    static void OnError(int errorCode, const char *msg);
    void Update();
    void Render();
    void Run();
    std::vector<const Model *> models;
};
