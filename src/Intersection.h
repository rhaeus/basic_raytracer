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

    float getDistance() const;
    std::shared_ptr<Renderable> getObject() const;
    const glm::vec3& getPosition() const;
    const glm::vec3& getNormal() const;
    const Ray& getRay() const;
    
    bool intersectionOccurred;

private:
    Ray ray;
    float distance;
    glm::vec3 position;
    
    
    glm::vec3 normal;
    std::shared_ptr<Renderable> object;


};