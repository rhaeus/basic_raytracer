#include "Intersection.h"

Intersection::Intersection(Ray ray)
    : Intersection(ray, 0.0f, glm::vec3(0,0,0), glm::vec3(0,0,0), nullptr, false)
{

}

Intersection::Intersection(Ray ray_, float distance_, glm::vec3 position_, glm::vec3 normal_, std::shared_ptr< Renderable> object_, bool intersectionOccurred_)
    : ray(ray_), distance(distance_), position(position_), normal(normal_), object(object_), intersectionOccurred(intersectionOccurred_)
{
    // position = position_ + normal_ * 0.01f;
}

float Intersection::getDistance() const
{
    return distance;
}

std::shared_ptr< Renderable> Intersection::getObject() const
{
    return object;
}

const glm::vec3& Intersection::getPosition() const
{
    return position;
}

const Ray& Intersection::getRay() const
{
    return ray;
}

const glm::vec3& Intersection::getNormal() const  
{
    return normal;
}