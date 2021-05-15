#include "Plane.h"

Plane::Plane(glm::vec3 lowerLeft, glm::vec3 uVec, glm::vec3 vVec, std::shared_ptr<Material> material_)
    : TriangleMesh(material_)
{
    std::vector<glm::vec3> vs;
    vs.push_back(lowerLeft);
    vs.push_back(lowerLeft + uVec);
    vs.push_back(lowerLeft + uVec + vVec);
    vs.push_back(lowerLeft + vVec);

    triangles.push_back(new Triangle(vs[0], vs[1], vs[2], glm::vec2(0, 1), glm::vec2(1, 1), glm::vec2(1, 0), material_));
    triangles.push_back(new Triangle(vs[0], vs[2], vs[3], glm::vec2(0, 1), glm::vec2(1, 0), glm::vec2(0, 0), material_));
}