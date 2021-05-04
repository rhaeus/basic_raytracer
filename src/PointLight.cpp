#include "PointLight.h"

PointLight::PointLight(glm::vec3 position, glm::vec3 color, float flux)
    : Light(position, color, flux)
{
    radiosity = color * flux;
}
