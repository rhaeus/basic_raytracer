#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "Material.h"
#include "Triangle.h"

class TriangleMesh 
{
public:
    TriangleMesh(std::shared_ptr<Material> material_) {
        material = material_;
    };

    std::shared_ptr<Material> getMaterial() {
        return material;
    }

    // void addTriangle(Triangle t) { triangles.push_back(t); };
    std::vector<Triangle*> getTriangles() { return triangles; };

protected:
    std::vector<Triangle*> triangles;
    std::shared_ptr<Material> material;
};