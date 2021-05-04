#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "Material.h"

class Ray
{
public:
    // Ray();
    Ray(glm::vec3 origin, glm::vec3 direction, float remainingReflections, std::shared_ptr<Material> travelMaterial);

    glm::vec3 getOrigin() const;
    glm::vec3 getDirection() const;
    // glm::vec3 getPoint(float lambda) ;
    float getRemainingReflections() const;
    std::shared_ptr<Material> getTravelMaterial() ;

private:
    glm::vec3 origin;
    glm::vec3 biasedOrigin;
    glm::vec3 direction;
    float remainingReflections;
    std::shared_ptr<Material> travelMaterial;
};