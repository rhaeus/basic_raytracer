#include "Ray.h"
// Ray::Ray()
//     : Ray(glm::vec3(0,0,0), glm::vec3(0,0,0), 0) 
// {}
        
Ray::Ray(glm::vec3 origin_, glm::vec3 direction_, float remainingReflections_, std::shared_ptr<Material> travelMaterial_)
    : origin(origin_), direction(direction_), remainingReflections(remainingReflections_), travelMaterial(travelMaterial_)
{
    origin = origin_;
    biasedOrigin = origin_ + direction_ / 10.0f; // add a bit of an offset to avoid intersecting immediately with same object
}

glm::vec3 Ray::getOrigin()  const
{
    return biasedOrigin;
}

glm::vec3 Ray::getDirection() const
{
    return direction;
}

// glm::vec3 Ray::getPoint(float lambda) 
// {
//     return origin + lambda * direction;
// }

float Ray::getRemainingReflections() const
{
    return remainingReflections;
}

std::shared_ptr<Material> Ray::getTravelMaterial() 
{
    return travelMaterial;
}