#include "Ray.h"
// Ray::Ray()
//     : Ray(glm::vec3(0,0,0), glm::vec3(0,0,0), 0) 
// {}
        
Ray::Ray(glm::vec3 origin_, glm::vec3 direction_, float remainingReflections_, std::shared_ptr<Material> travelMaterial_)
    : origin(origin_), direction(direction_), remainingReflections(remainingReflections_), travelMaterial(travelMaterial_)
{
    origin = origin_;
    dirFrac = 1.0f / direction_;
    biasedOrigin = origin_ + direction_ / 100.0f; // add a bit of an offset to avoid intersecting immediately with same object
}

const glm::vec3& Ray::getOrigin()  const
{
    return biasedOrigin;
}

const glm::vec3& Ray::getDirection() const
{
    return direction;
}

const glm::vec3& Ray::getDirFrac() const
{
    return dirFrac;
}

// glm::vec3 Ray::getPoint(float lambda) 
// {
//     return origin + lambda * direction;
// }

float Ray::getRemainingReflections() const
{
    return remainingReflections;
}

std::shared_ptr<Material> Ray::getTravelMaterial() const
{
    return travelMaterial;
}