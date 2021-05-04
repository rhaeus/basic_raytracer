#pragma once

#include <glm/glm.hpp>
#include <memory>

class Light : public std::enable_shared_from_this<Light>
{
public:
    Light(glm::vec3 position_, glm::vec3 color_, float flux_)
        : position(position_), color(color_), flux(flux_) { radiosity = color * flux; };

    virtual glm::vec3 getRadiosity()  { return radiosity; };

    glm::vec3 getPosition()  { return position; };
    glm::vec3 getColor()  { return color; };
    float getFlux()  { return flux; };

protected:
    glm::vec3 position;
    glm::vec3 color;
    float flux;
    glm::vec3 radiosity;
};