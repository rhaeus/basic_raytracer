#include "Turbulent.h"

#include <iostream>

Turbulent::Turbulent(glm::vec3 color1_, glm::vec3 color2_, float scale_, float shininess_, float reflectiveness_)
    : color1(color1_), color2(color2_), scale(scale_)
{
    shininess = shininess_;
    reflectiveness = reflectiveness_;
}

glm::vec3 Turbulent::getColor(const glm::vec3& pos, const glm::vec2& uv) 
{
    double x = pos.x * scale;
    double y = pos.y * scale;
    double z = pos.z * scale;

    double noiseCoef = 0.0;

    for (int level = 1; level < 10; level ++) {
        double noise = 
        noiseCoef += (1.0 / level) * fabs(perlinNoise.noise(
            level * 0.05 * x,
            level * 0.05 * y,
            level * 0.05 * z
        ));
    }

    if (noiseCoef > 1.0) {
        noiseCoef = 1.0;
    }

    return color1 * (float)noiseCoef + color2 * (float)(1.0 - noiseCoef);
}