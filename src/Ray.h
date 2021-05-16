#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "Material.h"

class Ray
{
public:
    // Ray();
    Ray(glm::vec3 origin, glm::vec3 direction, float remainingReflections, std::shared_ptr<Material> travelMaterial);

    const glm::vec3& getOrigin() const;
    const glm::vec3& getDirection() const;
    const glm::vec3& getDirFrac() const;
    // glm::vec3 getPoint(float lambda) ;
    float getRemainingReflections() const;
    std::shared_ptr<Material> getTravelMaterial() const;

private:
    glm::vec3 origin;
    glm::vec3 biasedOrigin;
    glm::vec3 direction;
    glm::vec3 dirFrac;
    float remainingReflections;
    std::shared_ptr<Material> travelMaterial;
};