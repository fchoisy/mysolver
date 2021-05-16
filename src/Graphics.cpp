

#include <iostream>

#include <GL/glew.h>                    // Runtime loading of OpenGL API functions
#include <GLFW/glfw3.h>                 // Windowing
#include <glm/gtc/matrix_transform.hpp> // Vector maths
#include <glm/gtx/string_cast.hpp>      // For debugging
#include <tdogl/Program.h>

#include "Graphics.hpp"
#include "helpers/RootDir.h"
#include "Physics.hpp"
// constants
const glm::vec2 SCREEN_SIZE(800, 600);

// globals
GLFWwindow *gWindow = NULL;
tdogl::Program *gProgram = NULL;

static void LoadShaders()
{
    std::vector<tdogl::Shader> shaders;
    shaders.push_back(tdogl::Shader::shaderFromFile(ROOT_DIR "/resources/vertex-shader.glsl", GL_VERTEX_SHADER));
    shaders.push_back(tdogl::Shader::shaderFromFile(ROOT_DIR "/resources/geometry-shader.glsl", GL_GEOMETRY_SHADER));
    shaders.push_back(tdogl::Shader::shaderFromFile(ROOT_DIR "/resources/fragment-shader.glsl", GL_FRAGMENT_SHADER));
    gProgram = new tdogl::Program(shaders);

    gProgram->use();
    // glm::mat4 projection = glm::perspective(glm::radians(50.0f), SCREEN_SIZE.x/SCREEN_SIZE.y, 0.1f, 10.0f);
    GLfloat aspect = SCREEN_SIZE.x / SCREEN_SIZE.y;
    glm::mat4 projection = glm::ortho(-aspect, aspect, -1.f, 1.0f);
    gProgram->setUniform("projection", projection);

    glm::mat4 camera = glm::translate(glm::mat4(1.0f), glm::vec3(-1.f, -.5f, 0.f));
    gProgram->setUniform("camera", camera);

    gProgram->stopUsing();
}

static Model *FluidModelFromParticles(const ParticleSet &particleSet)
{
    Model *fluidModel = new Model();
    glGenVertexArrays(1, &fluidModel->vao);
    glGenBuffers(1, &fluidModel->vbo);
    fluidModel->drawCount = particleSet.particles.size();

    glBindVertexArray(fluidModel->vao);

    glBindBuffer(GL_ARRAY_BUFFER, fluidModel->vbo);

    std::vector<GLfloat> vertexData;
    for (std::vector<Particle>::const_iterator it = particleSet.particles.begin(); it != particleSet.particles.end(); ++it)
    {
        // Position
        vertexData.push_back(it->position.x);
        vertexData.push_back(it->position.y);
        // Color
        vertexData.push_back(it->position.x);
        vertexData.push_back(it->position.y);
        vertexData.push_back(0.f);
        vertexData.push_back(1.f);
    }

    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);

    GLsizei stride = 6 * sizeof(GLfloat);
    const GLvoid *offset = (const GLvoid *)(2 * sizeof(GLfloat));

    glEnableVertexAttribArray(gProgram->attrib("vert"));
    glVertexAttribPointer(gProgram->attrib("vert"), 2, GL_FLOAT, GL_FALSE, stride, NULL);

    glEnableVertexAttribArray(gProgram->attrib("vertColor"));
    glVertexAttribPointer(gProgram->attrib("vertColor"), 4, GL_FLOAT, GL_TRUE, stride, offset);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return fluidModel;
}

static void Update()
{
    ;
}

static void Render(const Model &fluidModel)
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gProgram->use();

    glBindVertexArray(fluidModel.vao);

    // gProgram->setUniform("color", glm::vec4(0, 0, 1, 1));
    glDrawArrays(GL_POINTS, 0, fluidModel.drawCount);

    glBindVertexArray(0);

    gProgram->stopUsing();
    glfwSwapBuffers(gWindow);
}

void OnError(int errorCode, const char *msg)
{
    throw std::runtime_error(msg);
}

Graphics::Graphics(void (*OnInit)(Model *&fluidModel), void (*OnUpdate)(), void (*OnClose)())
    : _OnInit(OnInit), _OnUpdate(OnUpdate), _OnClose(OnClose)
{
}

Graphics::~Graphics()
{
}

void Graphics::run()
{
    // initialise GLFW
    glfwSetErrorCallback(OnError);
    if (!glfwInit())
        throw std::runtime_error("glfwInit failed");

    // open a window with GLFW
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    gWindow = glfwCreateWindow((int)SCREEN_SIZE.x, (int)SCREEN_SIZE.y, "OpenGL Tutorial", NULL, NULL);
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

    // load vertex and fragment shaders into opengl
    LoadShaders();

    Model *fluidModel;
    _OnInit(&fluidModel);

    // run while the window is open
    while (!glfwWindowShouldClose(gWindow))
    {
        // sleep until event
        glfwWaitEvents();

        _OnUpdate();
        Update();

        // draw one frame
        Render(*fluidModel);

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