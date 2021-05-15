#include "Cube.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

inline
float deg2rad(const float &deg)
{ return deg * M_PI / 180.0f; }

Cube::Cube(glm::vec3 center, std::shared_ptr<Material> material_, glm::vec3 scale, float roll_, float pitch_, float yaw_)
    : TriangleMesh(material_)
{
    float roll = deg2rad(roll_);
    float pitch = deg2rad(pitch_);
    float yaw = deg2rad(yaw_);

    glm::mat4 translation = glm::translate(glm::mat4(1.0), center);
    glm::mat4 scaling = glm::scale(glm::mat4(1.0), scale);
    glm::mat4 rotation = glm::yawPitchRoll(yaw, pitch, roll);
    glm::mat4 transformation = translation * rotation * scaling;

    std::vector<glm::vec4> vertices;
    vertices.push_back(glm::vec4(-0.5f, -0.5f, -0.5f, 1));
    vertices.push_back(glm::vec4(+0.5f, -0.5f, -0.5f, 1));
    vertices.push_back(glm::vec4(+0.5f, +0.5f, -0.5f, 1));
    vertices.push_back(glm::vec4(-0.5f, +0.5f, -0.5f, 1));

    vertices.push_back(glm::vec4(-0.5f, -0.5f, +0.5f, 1));
    vertices.push_back(glm::vec4(+0.5f, -0.5f, +0.5f, 1));
    vertices.push_back(glm::vec4(+0.5f, +0.5f, +0.5f, 1));
    vertices.push_back(glm::vec4(-0.5f, +0.5f, +0.5f, 1));

    // transform cube
    std::vector<glm::vec3> vs;
    for (auto v : vertices) 
    {
        glm::vec4 vertex = transformation * v;
        vs.push_back(glm::vec3(vertex.x/vertex.w, vertex.y/vertex.w, vertex.z/vertex.w)); 
    }

    // front
    triangles.push_back(new Triangle(vs[0], vs[1], vs[2], glm::vec2(0.25, 0.66), glm::vec2(0.50, 0.66), glm::vec2(0.50, 0.33), material_));
    triangles.push_back(new Triangle(vs[0], vs[2], vs[3], glm::vec2(0.25, 0.66), glm::vec2(0.50, 0.33), glm::vec2(0.25, 0.33), material_));

    //left
    triangles.push_back(new Triangle(vs[4], vs[0], vs[3], glm::vec2(0.00, 0.66), glm::vec2(0.25, 0.66), glm::vec2(0.25, 0.33), material_));
    triangles.push_back(new Triangle(vs[4], vs[3], vs[7], glm::vec2(0.00, 0.66), glm::vec2(0.25, 0.33), glm::vec2(0.00, 0.33), material_));

    // right
    triangles.push_back(new Triangle(vs[1], vs[5], vs[2], glm::vec2(0.50, 0.66), glm::vec2(0.75, 0.66), glm::vec2(0.50, 0.33),material_));
    triangles.push_back(new Triangle(vs[5], vs[6], vs[2], glm::vec2(0.75, 0.66), glm::vec2(0.75, 0.33), glm::vec2(0.55, 0.33),material_));

    //back
    triangles.push_back(new Triangle(vs[5], vs[4], vs[7], glm::vec2(0.75, 0.66), glm::vec2(1.00, 0.66), glm::vec2(1.00, 0.33),material_));
    triangles.push_back(new Triangle(vs[5], vs[7], vs[6], glm::vec2(0.75, 0.66), glm::vec2(1.00, 0.33), glm::vec2(0.75, 0.33),material_));

    // top
    triangles.push_back(new Triangle(vs[3], vs[2], vs[7], glm::vec2(0.25, 0.33), glm::vec2(0.50, 0.33), glm::vec2(0.25, 0.00),material_));
    triangles.push_back(new Triangle(vs[2], vs[6], vs[7], glm::vec2(0.50, 0.33), glm::vec2(0.50, 0.00), glm::vec2(0.25, 0.00),material_));

    // bottom
    triangles.push_back(new Triangle(vs[5], vs[1], vs[0], glm::vec2(0.50, 1.00), glm::vec2(0.50, 0.66), glm::vec2(0.25, 0.66),material_));
    triangles.push_back(new Triangle(vs[5], vs[0], vs[4], glm::vec2(0.50, 1.00), glm::vec2(0.25, 0.66), glm::vec2(0.25, 1.00),material_));

    // calculate bounds
    // bounds = triangles.at(0)->getBounds();
    // for (auto t : triangles) {
    //     bounds = bounds + t->getBounds();
    // }
}