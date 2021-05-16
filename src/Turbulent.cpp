#include "Turbulent.h"

#include <iostream>

Turbulent::Turbulent(glm::vec3 color1_, glm::vec3 color2_, float scale_)
    : color1(color1_), color2(color2_), scale(scale_)
{}

glm::vec3 Turbulent::getColor(glm::vec3 pos, glm::vec2 uv) 
{
    float x = pos.x * scale;
    float y = pos.y * scale;
    float z = pos.z * scale;

    float noiseCoef = 0;

    for (int level = 1; level < 10; level ++) {
        noiseCoef += (1.0f / level) * fabsf(perlinNoise.noise(
            level * 0.05 * x,
            level * 0.05 * y,
            level * 0.05 * z
        ));
    }

    return color1 * noiseCoef + color2 * (1.0f - noiseCoef);
}