#pragma once

#include <vector>
#include "Model.hpp"

class Graphics
{
private:
    void (*_OnInit)();
    void (*_OnUpdate)();
    void (*_OnClose)();
    const glm::vec2 SCREEN_SIZE;
    GLFWwindow *gWindow;

public:
    Graphics(void (*OnInit)(), void (*OnUpdate)(), void (*OnClose)());
    ~Graphics();
    static void OnError(int errorCode, const char *msg);
    void Update();
    void Render();
    void Run();
    std::vector<const Model*> models;
};
