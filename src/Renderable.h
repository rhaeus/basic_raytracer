#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <memory>

#include "Ray.h"
#include "Intersection.h"
#include "Material.h"
#include "AABB.h"

class Renderable : public std::enable_shared_from_this<Renderable>
{
public:
    Renderable(std::shared_ptr<Material> material_) 
        : material(material_) {};

    virtual Intersection intersect(const Ray& ray) = 0;
    // { 
    //     std::cout << "SceneObject intersect" << std::endl;
    //     return false; 
    // };
    std::shared_ptr<Material> getMaterial() { return material; }
    virtual glm::vec3 getColor(glm::vec3 pos)  = 0;
    virtual glm::vec3 getNormal(glm::vec3 pos)  = 0;
    std::shared_ptr<AABB> getBounds() { return bounds; };

protected:
    std::shared_ptr<AABB> bounds;
    std::shared_ptr<Material> material;
    
};