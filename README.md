# Simple Fluid Solver - Work in Progress

This project aims at implementing a fluid solver based on the SPH (Smoothed Particle Hydrodynamics) method.
The goal of the project is to better understand how the the speed and accuracy of such a fluid simulation are influenced by the different parameters of the simulation, and in particular by the time step, the viscosity and the stiffness.

The quality of the code is **not** what is evaluated in this project, it is mostly used for *experiments*.
This means that there might be some bad code practices and memory leaks... Although I am working towards reducing them.

## Third-party dependencies
- GLEW: for the runtime handling of OpenGL methods.
- GLFW: for the window and keyboard interaction.
- GLM: for matrix and vector math.
- tdogl: from Tom Dalling's OpenGL tutorial.

For now, the project has only been compiled and tested in a macOS (Intel) environment.
