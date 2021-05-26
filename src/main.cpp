/**
 * main
 * 
 * Tom Dalling's OpenGL tutorial (http://tomdalling.com/) chapter 1 was taken as starting point for this file.
 */

// third-party libraries

// standard C++ libraries
#include <iostream>

#include <GL/glew.h>                    // Runtime loading of OpenGL API functions
#include <GLFW/glfw3.h>                 // Windowing
#include <glm/gtc/matrix_transform.hpp> // Vector maths
#include <glm/gtx/string_cast.hpp>      // For debugging
#include <tdogl/Program.h>

#include "Graphics.hpp"
#include "Particles.hpp"
#include "Model.hpp"
#include "Algorithms.hpp"
#include "helpers/RootDir.h"

// GLuint gVAO = 0;
// GLuint gVBO = 0;

// static void FindAllNeighbors(ParticleSet &particleSet)
// {
//     // Naive O(n^2)
//     for (std::vector<Particle>::iterator it = particleSet.particles.begin(); it != particleSet.particles.end(); ++it)
//     {
//         (*it).neighbors = *(new std::vector<const Particle *>());
//         for (std::vector<Particle>::const_iterator it2 = particleSet.particles.begin(); it2 != particleSet.particles.end(); ++it2)
//         {
//             if (it.base() != it2.base() && glm::distance((*it).position, (*it2).position) > particleSet.particleSpacing)
//             {
//                 (*it).neighbors.push_back(it2.base());
//             }
//         }
//     }
// }

Model *gModel = NULL;
Graphics *gGraphics = NULL;

void MyOnInit()
{
    // Init particle set
    ParticleSet particleSet;
    particleSet.particles = std::vector<Particle>();
    particleSet.particleSpacing = 0.1f;
    for (size_t i = 0; i < 10; i++)
    {
        for (size_t j = 0; j < 10; j++)
        {
            Particle part;
            part.position = glm::vec2(i * particleSet.particleSpacing, j * particleSet.particleSpacing);
            particleSet.particles.push_back(part);
        }
    }
    // Find neighbors for each particle position
    std::vector<std::vector<const Particle *> *> *allNeighbors = FindAllNeighbors(particleSet);
    //
    for (auto &&it = allNeighbors->begin(); it != allNeighbors->end(); ++it)
    {
        std::cout << (*it)->size() << " ";
        if (((it - allNeighbors->begin() + 1) % 10) == 0)
        {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
    // Find neighbors for one particular position
    Particle &someParticle = particleSet.particles[6 * 10 + 3];
    std::cout << "My position: " << glm::to_string(someParticle.position) << std::endl;
    std::vector<const Particle *> *neighbors = (*allNeighbors)[6 * 10 + 3];
    for (auto &&it = neighbors->begin(); it != neighbors->end(); ++it)
    {
        std::cout << "Neighbor " << it - neighbors->begin() + 1 << ": " << glm::to_string((*it)->position) << std::endl;
    }

    // create buffer and fill it with the points of the cube
    // LoadVertexData(particleSet);
    // *fluidModel = FluidModelFromParticles(particleSet);
    gModel = new Model(*particleSet.ToVertexData(),
                       ROOT_DIR "resources/vertex-shader.glsl",
                       ROOT_DIR "resources/geometry-shader.glsl",
                       ROOT_DIR "resources/fragment-shader.glsl");

    gGraphics->models.push_back(gModel);
    std::cout << "Just initialized" << std::endl;
}

void MyOnUpdate()
{
    // std::cout << "Just updated" << std::endl;
}

void MyOnClose()
{
    if (gModel)
        delete gModel;
    std::cout << "Just closed" << std::endl;
}

// the program starts here
void AppMain()
{
    // void (*OnInit)() = &MyOnInit;
    // void (*OnUpdate)() = &MyOnUpdate;
    // void (*OnClose)() = &MyOnClose;
    gGraphics = new Graphics(&MyOnInit, &MyOnUpdate, &MyOnClose);
    gGraphics->Run();
    delete gGraphics;
}

int main(int argc, char *argv[])
{
    try
    {
        AppMain();
    }
    catch (const std::exception &e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
