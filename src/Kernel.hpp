#pragma once

#include <vector>
#include <glm/vec2.hpp>

class Kernel
{
private:
    std::vector<float> functionVertexData;
    std::vector<float> derivativeVertexData;
    const float h;

public:
    Kernel(const float h);
    float Function(const glm::vec2 &position_i, const glm::vec2 &position_j);
    glm::vec2 Derivative(const glm::vec2 &position_i, const glm::vec2 &position_j);
    const std::vector<float> &FunctionVertexData();
    const std::vector<float> &DerivativeVertexData();
};

// float KernelFunction(, const float &h);
// glm::vec2 KernelDerivative(const glm::vec2 &position_i, const glm::vec2 &position_j, const float &h);
