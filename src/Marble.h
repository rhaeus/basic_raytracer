#pragma once

#include "Material.h"
#include "PerlinNoise.h"

class Marble : public Material
{
public:
    Marble(glm::vec3 color1_, glm::vec3 color2_, float scale_ = 1.0f, float shininess_ = -1.0, float reflectiveness_=-1.0);
    glm::vec3 getColor(const glm::vec3& pos, const glm::vec2& uv) override;

private:
    glm::vec3 color1;
    glm::vec3 color2;
    float scale;
    PerlinNoise perlinNoise;

};