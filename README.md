# Simple Fluid Solver

This project aims at implementing a fluid solver based on the SPH (Smoothed Particle Hydrodynamics) method.
The goal of the project is to better understand how the the speed and accuracy of such a fluid simulation are influenced by the different parameters of the simulation, and in particular by the time step, the viscosity and the stiffness.

## Third-party dependencies
- GLEW: for the runtime handling of OpenGL methods.
- GLFW: for the window and keyboard interaction.
- GLM: for matrix and vector math.
- tdogl: from Tom Dalling's OpenGL tutorial.

For now, the project has only been compiled and tested in a macOS (Intel) environment.

## To-do


- neighbor search
    - [x] naively
    - [ ] naively with unordered pairs (triangularly)
    - [ ] efficiently
    - [ ] visualize
- [ ] kernel function and derivative
- [x] set up visualization framework
- [x] import required libraries
