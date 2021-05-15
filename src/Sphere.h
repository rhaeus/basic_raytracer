#pragma once

#include "Renderable.h"
#include <memory>

extern unsigned long long raySphereIntersectionTest ;
extern unsigned long long raySphereIntersections;

class Sphere : public Renderable
{
public:
    Sphere(glm::vec3 center, float radius, std::shared_ptr<Material> material);
    
    Intersection intersect(const Ray& ray) override;
    glm::vec3 getColor(glm::vec3 pos)  override;
    glm::vec3 getNormal(glm::vec3 pos)  override;

private:
    glm::vec3 center;
    float radius;

};