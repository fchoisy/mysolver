# Simple Fluid Solver

This project aims at implementing a fluid solver based on the SPH (Smoothed Particle Hydrodynamics) method.
The goal of the project is to better understand how the the speed and accuracy of such a fluid simulation are influenced by the different parameters of the simulation, and in particular by the time step, the viscosity and the stiffness.

## Compile and run

Compile using CMake. CMakeLists.txt file might have to be adapted for different platforms.
Run produced executable using:

```
./build/mysolver
```


## Third-party dependencies
- GLEW: for the runtime handling of OpenGL methods.
- GLFW: for the window and keyboard interaction.
- GLM: for matrix and vector math.
- tdogl: from Tom Dalling's OpenGL tutorial.
- Catch2: for testing.

For now, the project has only been compiled and tested in a macOS (Intel CPU) environment.
