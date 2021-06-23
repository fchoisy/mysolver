#include "Kernel.hpp"

#include <glm/vec2.hpp>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>
#include <limits>
#include <exception>

Kernel::Kernel(const float h)
    : h(h)
{
    if (h < 0)
        throw std::invalid_argument("h must be >= 0");
}

float Kernel::Function(const glm::vec2 &position_i, const glm::vec2 &position_j)
{
    if (h < 0)
        throw std::invalid_argument("h must be >= 0");
    // float alpha = 1.f / (6.f * h);  // 1D
    float alpha = 5.f / (14.f * glm::pi<float>() * h * h); // 2D
    // float alpha = 1.f / (4.f * glm::pi<float>() * h * h * h);  // 3D
    float q = glm::distance(position_i, position_j) / h;
    float t1 = glm::max(1.f - q, 0.f);
    float t2 = glm::max(2.f - q, 0.f);
    return alpha * (t2 * t2 * t2 - 4 * t1 * t1 * t1);
}

glm::vec2 Kernel::Derivative(const glm::vec2 &position_i, const glm::vec2 &position_j)
{
    if (h < 0)
        throw std::invalid_argument("h must be >= 0");
    // float alpha = 1.f / (6.f * h);  // 1D
    float alpha = 5.f / (14.f * glm::pi<float>() * h * h); // 2D
    // float alpha = 1.f / (4.f * glm::pi<float>() * h * h * h);  // 3D
    float d = glm::distance(position_i, position_j) / h;
    if (d == 0)
    {
        return glm::vec2(0, 0);
    }
    float t1 = glm::max(1.f - d, 0.f);
    float t2 = glm::max(2.f - d, 0.f);
    return alpha * (position_i - position_j) / (d * h * h) * (-3 * t2 * t2 + 12 * t1 * t1);
}

const std::vector<float> &Kernel::FunctionVertexData()
{
    functionVertexData.clear();
    glm::vec2 origin(0, 0);
    const int numSteps = 100;
    const float rangeStart = -8.f;
    const float rangeStop = 8.f;
    const float stepSize = (rangeStop - rangeStart) / numSteps;
    for (int i = 0; i < numSteps; i++)
    {
        float x = rangeStart + stepSize * i;
        float y = Function(origin, glm::vec2(x, 0.f)) * 10.f;
        // std::cout << y << std::endl;
        functionVertexData.push_back(x);
        functionVertexData.push_back(y);
        functionVertexData.push_back(1);
        functionVertexData.push_back(1);
        functionVertexData.push_back(1);
        functionVertexData.push_back(1);
    }
    return functionVertexData;
}

const std::vector<float> &Kernel::DerivativeVertexData()
{
    derivativeVertexData.clear();
    glm::vec2 origin(0, 0);
    const int numSteps = 100;
    const float rangeStart = -8.f;
    const float rangeStop = 8.f;
    const float stepSize = (rangeStop - rangeStart) / numSteps;
    for (int i = 0; i < numSteps; i++)
    {
        float x = rangeStart + stepSize * i;
        float y = Derivative(origin, glm::vec2(x, 0.f)).x * 10.f;
        derivativeVertexData.push_back(x);
        derivativeVertexData.push_back(y);
        derivativeVertexData.push_back(1);
        derivativeVertexData.push_back(1);
        derivativeVertexData.push_back(1);
        derivativeVertexData.push_back(1);
    }
    return derivativeVertexData;
}