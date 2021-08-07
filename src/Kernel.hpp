#pragma once

#include <vector>
#include <glm/vec2.hpp>

// Represents the SPH kernel function (here a cubic spline).
class Kernel
{
public:
    // h is the radius of the kernel support
    Kernel(const float h);
    // Evaluates the kernel function for two positions.
    float Function(const glm::vec2 &position_i, const glm::vec2 &position_j);
    // Evaluates the kernel function derivative for two positions.
    glm::vec2 Derivative(const glm::vec2 &position_i, const glm::vec2 &position_j);
    // Returns the value of the kernel function over an interval, for plotting.
    const std::vector<float> &FunctionVertexData();
    // Returns the value of the kernel function derivative over an interval, for plotting.
    const std::vector<float> &DerivativeVertexData();

private:
    std::vector<float> functionVertexData;
    std::vector<float> derivativeVertexData;
    const float h;
    float alpha;
};
