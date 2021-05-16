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
#include "Physics.hpp"

// GLuint gVAO = 0;
// GLuint gVBO = 0;

static void FindNeighbors(Particle &particle, ParticleSet &particleSet)
{
    float searchRadius = 2 * particleSet.particleSpacing;
    particle.neighbors = std::vector<const Particle *>();
    for (std::vector<Particle>::const_iterator it2 = particleSet.particles.begin(); it2 != particleSet.particles.end(); ++it2)
    {
        if (&particle != it2.base() && glm::distance(particle.position, (*it2).position) < searchRadius)
        {
            particle.neighbors.push_back(it2.base());
        }
    }
}

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

void MyOnInit(Model **fluidModel)
{
    ParticleSet particleSet;
    particleSet.particles = std::vector<Particle>();
    particleSet.particleSpacing = 0.1f;
    for (size_t i = 0; i < 10; i++)
    {
        for (size_t j = 0; j < 10; j++)
        {
            Particle part = {
                .position = glm::vec2(i * particleSet.particleSpacing, j * particleSet.particleSpacing)};
            particleSet.particles.push_back(part);
        }
    }

    Particle &frontParticle = particleSet.particles.front();
    FindNeighbors(frontParticle, particleSet);
    for (std::vector<const Particle *>::const_iterator it = frontParticle.neighbors.begin(); it != frontParticle.neighbors.end(); ++it)
    {
        std::cout << glm::to_string((*it)->position) << std::endl;
    }

    // create buffer and fill it with the points of the cube
    // LoadVertexData(particleSet);
    *fluidModel = FluidModelFromParticles(particleSet);
    std::cout << "Just initialized" << std::endl;
}

void MyOnUpdate()
{
    std::cout << "Just updated" << std::endl;
}

void MyOnClose()
{
    std::cout << "Just closed" << std::endl;
}

// the program starts here
void AppMain()
{
    // void (*OnInit)() = &MyOnInit;
    // void (*OnUpdate)() = &MyOnUpdate;
    // void (*OnClose)() = &MyOnClose;
    Graphics graphics(&MyOnInit, &MyOnUpdate, &MyOnClose);
    graphics.run();
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
