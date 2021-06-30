#include <iostream>

#include <GL/glew.h>                    // Runtime loading of OpenGL API functions
#include <GLFW/glfw3.h>                 // Windowing
#include <glm/common.hpp>               // Vector maths
#include <glm/gtc/matrix_transform.hpp> // Vector maths
// #include <glm/gtx/string_cast.hpp>      // For debugging

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Graphics.hpp"

Graphics::Graphics(Experiment &experiment)
    : experiment(experiment), SCREEN_SIZE(800, 600), internalState{.5f}
{
}

Graphics::~Graphics()
{
}

void Graphics::OnError(int errorCode, const char *msg)
{
    throw std::runtime_error(msg);
}

void Graphics::Update()
{
    if (glfwGetKey(gWindow, GLFW_KEY_SPACE))
    {
        experiment.OnUpdate();
    }
}

void Graphics::Render()
{
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto &&model : models)
    {
        model->program->use();
        glBindVertexArray(model->vao);
        GLfloat aspect = 800.f / 600.f;
        glm::mat4 projection = glm::ortho(-aspect, aspect, -1.f, 1.0f);
        model->program->setUniform("projection", projection);

        glm::mat4 camera = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-.2f, -.2f, 0.f)), glm::vec3(internalState.zoomLevel, internalState.zoomLevel, internalState.zoomLevel));
        model->program->setUniform("camera", camera);

        glDrawArrays(model->drawMode, 0, model->drawCount);
        glBindVertexArray(0);
        model->program->stopUsing();
    }

    experiment.OnRender();
    // // render GUI
    // ImGui::Begin("Demo window");
    // ImGui::Button("Hello!");
    // ImGui::End();

    // Render dear imgui into screen
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(gWindow);
}

void OnScroll(GLFWwindow *window, double xoffset, double yoffset)
{
    // struct internalState
    InternalState *internalState = (InternalState *)glfwGetWindowUserPointer(window);
    // if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
    // {
    //     internalState->cameraX += yoffset;
    // }
    // else
    internalState->zoomLevel = glm::clamp((float)(internalState->zoomLevel + yoffset * .5f), 0.1f, 10.f);
}

void Graphics::Run()
{
    // initialise GLFW
    glfwSetErrorCallback(Graphics::OnError);
    if (!glfwInit())
        throw std::runtime_error("glfwInit failed");

    // open a window with GLFW
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    gWindow = glfwCreateWindow((int)SCREEN_SIZE.x, (int)SCREEN_SIZE.y, "Fluid Solver", NULL, NULL);
    if (!gWindow)
        throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.2?");

    // GLFW settings
    glfwMakeContextCurrent(gWindow);
    glfwSetWindowUserPointer(gWindow, &(this->internalState));
    glfwSetScrollCallback(gWindow, OnScroll);

    // initialise GLEW
    glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
    if (glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");

    // GLEW throws some errors, so discard all the errors so far
    while (glGetError() != GL_NO_ERROR)
    {
    }

    // print out some info about the graphics drivers
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    // make sure OpenGL version 3.2 API is available
    if (!GLEW_VERSION_3_2)
        throw std::runtime_error("OpenGL 3.2 API is not available.");

    // OpenGL settings
    glEnable(GL_DEPTH_TEST); // Enable depth buffering
    glDepthFunc(GL_LESS);    // The pixels with less depth will be drawn on top
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    experiment.OnInit();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(gWindow, true);
    ImGui_ImplOpenGL3_Init("#version 150");
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // run while the window is open
    while (!glfwWindowShouldClose(gWindow))
    {
        // sleep until event
        glfwWaitEvents();

        // feed inputs to dear imgui, start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        Update();

        // draw one frame
        Render();

        // check for errors
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
            std::cerr << "OpenGL Error " << error << std::endl;

        if (glfwGetKey(gWindow, GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(gWindow, GL_TRUE);
        }
    }
    experiment.OnClose();

    // clean up and exit
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(gWindow);
    glfwTerminate();
}