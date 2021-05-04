#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "Ray.h"
#include "Material.h"

class Renderable;

class Intersection
{
public:
    Intersection(Ray ray);
    Intersection(Ray ray, float distance, glm::vec3 position, glm::vec3 normal, std::shared_ptr< Renderable> object, bool intersectionOccurred);

    float getDistance() ;
    std::shared_ptr<Renderable> getObject() const;
    glm::vec3 getPosition() const;
    glm::vec3 getNormal() const;
    Ray getRay() const;
    glm::vec3 position;
    bool intersectionOccurred;

private:
    Ray ray;
    float distance;
    
    
    
    glm::vec3 normal;
    std::shared_ptr<Renderable> object;


};