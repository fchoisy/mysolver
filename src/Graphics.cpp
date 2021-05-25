#include <iostream>

#include <GL/glew.h>                    // Runtime loading of OpenGL API functions
#include <GLFW/glfw3.h>                 // Windowing
#include <glm/gtc/matrix_transform.hpp> // Vector maths
#include <glm/gtx/string_cast.hpp>      // For debugging

#include "Graphics.hpp"
#include "Particles.hpp"

Graphics::Graphics(void (*OnInit)(), void (*OnUpdate)(), void (*OnClose)())
    : _OnInit(OnInit), _OnUpdate(OnUpdate), _OnClose(OnClose), SCREEN_SIZE(800, 600)
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
    ;
}

void Graphics::Render()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (std::vector<const Model *>::const_iterator it = this->models.begin(); it != this->models.end(); ++it)
    {
        (*it)->program->use();
        glBindVertexArray((*it)->vao);
        // (*it)->program->setUniform("color", glm::vec4(0, 0, 1, 1));
        glDrawArrays(GL_POINTS, 0, (*it)->drawCount);
        glBindVertexArray(0);
        (*it)->program->stopUsing();
    }
    glfwSwapBuffers(gWindow);
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

    _OnInit();

    // run while the window is open
    while (!glfwWindowShouldClose(gWindow))
    {
        // sleep until event
        glfwWaitEvents();

        _OnUpdate();
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
    _OnClose();

    // clean up and exit
    glfwTerminate();
}