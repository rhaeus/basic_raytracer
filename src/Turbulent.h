#pragma once

#include "Material.h"
#include "PerlinNoise.h"

class Turbulent : public Material
{
public:
    Turbulent(glm::vec3 color1_, glm::vec3 color2_, float scale_ = 1.0f);
    glm::vec3 getColor(glm::vec3 pos, glm::vec2 uv) override;

private:
    glm::vec3 color1;
    glm::vec3 color2;
    float scale;
    PerlinNoise perlinNoise;

};