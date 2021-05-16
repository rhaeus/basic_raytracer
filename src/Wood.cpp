#include "Wood.h"

#include <iostream>

Wood::Wood(glm::vec3 color1_, glm::vec3 color2_, float scale_, float shininess_, float reflectiveness_)
    : color1(color1_), color2(color2_), scale(scale_)
{
    shininess = shininess_;
    reflectiveness = reflectiveness_;
}

glm::vec3 Wood::getColor(const glm::vec3& pos, const glm::vec2& uv) 
{
    double x = pos.x * scale;
    double y = pos.y * scale;
    double z = pos.z * scale;

    double grain = perlinNoise.noise(x, y, z) * 5;

    grain = grain - (int)grain;

    return color1 * (float)grain + color2 * (float)(1.0 - grain);
}