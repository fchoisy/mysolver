#include "Kernel.hpp"

#include <glm/vec2.hpp>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>
#include <limits>
#include <exception>

float KernelFunction(const glm::vec2 &position_i, const glm::vec2 &position_j, const float &h)
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

glm::vec2 KernelDerivative(const glm::vec2 &position_i, const glm::vec2 &position_j, const float &h)
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