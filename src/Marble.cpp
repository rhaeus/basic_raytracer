#include "Marble.h"

#include <iostream>

Marble::Marble(glm::vec3 color1_, glm::vec3 color2_, float scale_, float shininess_, float reflectiveness_)
    : color1(color1_), color2(color2_), scale(scale_)
{
    shininess = shininess_;
    reflectiveness = reflectiveness_;
}

glm::vec3 Marble::getColor(const glm::vec3& pos, const glm::vec2& uv) 
{
    float x = pos.x * scale;
    float y = pos.y * scale;
    float z = pos.z * scale;
    float noiseCoef = 0;

    for (int level = 1; level < 10; level ++) {
        noiseCoef +=  (1.0f / level) * fabsf(perlinNoise.noise(
            level * 0.05 * x,
            level * 0.15 * y,
            level * 0.10 * z
        ));
    }
    noiseCoef = 0.5f * sinf((x + y) * 0.05f + noiseCoef) + 0.5f;

    if (noiseCoef > 1.0) {
        noiseCoef = 1.0;
    }

    return color1 * noiseCoef + color2 * (1.0f - noiseCoef);
}