/**
 * main
 * 
 * Tom Dalling's OpenGL tutorial (http://tomdalling.com/) chapter 1 was taken as starting point for this file.
 */

// third-party libraries
#include <GL/glew.h>  // Runtime loading of OpenGL API functions
#include <GLFW/glfw3.h>  // Windowing
#include <glm/gtc/matrix_transform.hpp> // Vector maths
#include <tdogl/Program.h>
// standard C++ libraries
#include <iostream>

#include "helpers/RootDir.h"

struct Particle {
    glm::vec2 position, velocity;
    std::vector<Particle*> neighbors;
};

// struct ModelInstance {
//     tdogl::Program* shaders;

// };

struct ParticleFluid {
    std::vector<Particle> particles; 
};


// constants
const glm::vec2 SCREEN_SIZE(800, 600);

// globals
GLFWwindow* gWindow = NULL;
tdogl::Program* gProgram = NULL;
GLuint gVAO = 0;
GLuint gVBO = 0;


static void LoadShaders() {
    std::vector<tdogl::Shader> shaders;
    shaders.push_back(tdogl::Shader::shaderFromFile(ROOT_DIR"/resources/vertex-shader.glsl", GL_VERTEX_SHADER));
    shaders.push_back(tdogl::Shader::shaderFromFile(ROOT_DIR"/resources/geometry-shader.glsl", GL_GEOMETRY_SHADER));
    shaders.push_back(tdogl::Shader::shaderFromFile(ROOT_DIR"/resources/fragment-shader.glsl", GL_FRAGMENT_SHADER));
    gProgram = new tdogl::Program(shaders);
    
    gProgram->use();
    // glm::mat4 projection = glm::perspective(glm::radians(50.0f), SCREEN_SIZE.x/SCREEN_SIZE.y, 0.1f, 10.0f);
    GLfloat aspect = SCREEN_SIZE.x / SCREEN_SIZE.y;
    glm::mat4 projection = glm::ortho(-aspect, aspect, -1.f, 1.0f);
    gProgram->setUniform("projection", projection);
    gProgram->stopUsing();
}

static void LoadVertexData(const ParticleFluid& particleFluid) {
    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);
    
    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);

    // GLfloat vertexData[] = {
    //     -0.8f,-0.4f,
    //      0.0f, 0.4f,
    //      0.8f,-0.4f,
    // };

    std::vector<GLfloat> vertexData;
    std::vector<Particle>::const_iterator it;
    for (it = particleFluid.particles.begin(); it != particleFluid.particles.end(); ++it)
    {
        vertexData.push_back(it->position.x);
        vertexData.push_back(it->position.y);
    }
    
    
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);
    
    GLsizei stride = 0 * sizeof(GLfloat);
    
    glEnableVertexAttribArray(gProgram->attrib("vert"));
    glVertexAttribPointer(gProgram->attrib("vert"), 2, GL_FLOAT, GL_FALSE, stride, NULL);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

static void Update(float secondsElapsed) {
    ;
}

static void Render() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gProgram->use();
    
    glBindVertexArray(gVAO);
    
    gProgram->setUniform("color", glm::vec4(0, 0, 1, 1));
    glDrawArrays(GL_POINTS, 0, 1);
    
    glBindVertexArray(0);
    
    gProgram->stopUsing();
    glfwSwapBuffers(gWindow);
}

void OnError(int errorCode, const char* msg) {
    throw std::runtime_error(msg);
}

// the program starts here
void AppMain() {
    // initialise GLFW
    glfwSetErrorCallback(OnError);
    if(!glfwInit())
        throw std::runtime_error("glfwInit failed");
    
    // open a window with GLFW
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    gWindow = glfwCreateWindow((int)SCREEN_SIZE.x, (int)SCREEN_SIZE.y, "OpenGL Tutorial", NULL, NULL);
    if(!gWindow)
        throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.2?");

    // GLFW settings
    glfwMakeContextCurrent(gWindow);

    // initialise GLEW
    glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");
    
    // GLEW throws some errors, so discard all the errors so far
    while(glGetError() != GL_NO_ERROR) {}

    // print out some info about the graphics drivers
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    // make sure OpenGL version 3.2 API is available
    if(!GLEW_VERSION_3_2)
        throw std::runtime_error("OpenGL 3.2 API is not available.");

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);  // Enable depth buffering
    glDepthFunc(GL_LESS);  // The pixels with less depth will be drawn on top
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // load vertex and fragment shaders into opengl
    LoadShaders();

    ParticleFluid particleFluid;
    particleFluid.particles = std::vector<Particle>();
    Particle part = {
        .position = glm::vec2(.5f, .5f)
    };
    particleFluid.particles.push_back(part);

    // create buffer and fill it with the points of the cube
    LoadVertexData(particleFluid);

    // run while the window is open
    double lastTime = glfwGetTime();
    while(!glfwWindowShouldClose(gWindow)){
        // process pending events
        glfwPollEvents();
        
        // update the scene based on the time elapsed since last update
        double thisTime = glfwGetTime();
        Update((float)(thisTime - lastTime));
        lastTime = thisTime;


        // draw one frame
        Render();

        // check for errors
        GLenum error = glGetError();
        if(error != GL_NO_ERROR)
            std::cerr << "OpenGL Error " << error << std::endl;

        if (glfwGetKey(gWindow, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(gWindow, GL_TRUE);
        }
    }

    // clean up and exit
    glfwTerminate();
}


int main(int argc, char *argv[]) {
    try {
        AppMain();
    } catch (const std::exception& e){
        std::cerr << "ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
