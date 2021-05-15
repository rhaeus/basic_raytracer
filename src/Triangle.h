#pragma once

#include "Renderable.h"
#include <vector>

extern unsigned long long rayTriangleIntersectionTest;
extern unsigned long long rayTriangleIntersections;

class Triangle : public Renderable
{
public: 
    Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, std::shared_ptr<Material> material);
    Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec2 vt0, glm::vec2 vt1, glm::vec2 vt2, std::shared_ptr<Material> material);
    
    Intersection intersect(const Ray& ray) override;
    glm::vec3 getColor(glm::vec3 pos)  override;
    glm::vec3 getNormal(glm::vec3 pos)  override;

private:
    std::vector<glm::vec3> vertexCoords;
    glm::vec3 normal;
    std::vector<glm::vec2> textureCoords;

};