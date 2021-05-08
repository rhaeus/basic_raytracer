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

    glm::vec4 bla = transformation * glm::vec4(0,0,0,1);

    std::vector<glm::vec4> vertices;
    vertices.push_back(glm::vec4(-0.5f, -0.5f, -0.5f, 1));
    vertices.push_back(glm::vec4(+0.5f, -0.5f, -0.5f, 1));
    vertices.push_back(glm::vec4(+0.5f, +0.5f, -0.5f, 1));
    vertices.push_back(glm::vec4(-0.5f, +0.5f, -0.5f, 1));

    vertices.push_back(glm::vec4(-0.5f, -0.5f, +0.5f, 1));
    vertices.push_back(glm::vec4(+0.5f, -0.5f, +0.5f, 1));
    vertices.push_back(glm::vec4(+0.5f, +0.5f, +0.5f, 1));
    vertices.push_back(glm::vec4(-0.5f, +0.5f, +0.5f, 1));

    // vertices.push_back(glm::vec4(center.x-0.5f, center.y-0.5f, center.z-0.5f, 1));
    // vertices.push_back(glm::vec4(center.x+0.5f, center.y-0.5f, center.z-0.5f, 1));
    // vertices.push_back(glm::vec4(center.x+0.5f, center.y+0.5f, center.z-0.5f, 1));
    // vertices.push_back(glm::vec4(center.x-0.5f, center.y+0.5f, center.z-0.5f, 1));

    // vertices.push_back(glm::vec4(center.x-0.5f, center.y-0.5f, center.z+0.5f, 1));
    // vertices.push_back(glm::vec4(center.x+0.5f, center.y-0.5f, center.z+0.5f, 1));
    // vertices.push_back(glm::vec4(center.x+0.5f, center.y+0.5f, center.z+0.5f, 1));
    // vertices.push_back(glm::vec4(center.x-0.5f, center.y+0.5f, center.z+0.5f, 1));
    // vertices.push_back(glm::vec4(center.x-0.5f*200.0f, center.y-0.5f*200.0f, center.z-0.5f*200.0f, 1));
    // vertices.push_back(glm::vec4(center.x+0.5f*200.0f, center.y-0.5f*200.0f, center.z-0.5f*200.0f, 1));
    // vertices.push_back(glm::vec4(center.x+0.5f*200.0f, center.y+0.5f*200.0f, center.z-0.5f*200.0f, 1));
    // vertices.push_back(glm::vec4(center.x-0.5f*200.0f, center.y+0.5f*200.0f, center.z-0.5f*200.0f, 1));

    // vertices.push_back(glm::vec4(center.x-0.5f*200.0f, center.y-0.5f*200.0f, center.z+0.5f*200.0f, 1));
    // vertices.push_back(glm::vec4(center.x+0.5f*200.0f, center.y-0.5f*200.0f, center.z+0.5f*200.0f, 1));
    // vertices.push_back(glm::vec4(center.x+0.5f*200.0f, center.y+0.5f*200.0f, center.z+0.5f*200.0f, 1));
    // vertices.push_back(glm::vec4(center.x-0.5f*200.0f, center.y+0.5f*200.0f, center.z+0.5f*200.0f, 1));

    // transform cube
    std::vector<glm::vec3> vs;
    for (auto v : vertices) 
    {
        glm::vec4 vertex = transformation * v;
        vs.push_back(glm::vec3(vertex.x/vertex.w, vertex.y/vertex.w, vertex.z/vertex.w)); 
    }

    // build mesh

    // front
    triangles.push_back(new Triangle(vs[0], vs[1], vs[2], material_));
    triangles.push_back(new Triangle(vs[0], vs[2], vs[3], material_));

    //left
    triangles.push_back(new Triangle(vs[4], vs[0], vs[3], material_));
    triangles.push_back(new Triangle(vs[4], vs[3], vs[7], material_));

    // right
    triangles.push_back(new Triangle(vs[1], vs[5], vs[2], material_));
    triangles.push_back(new Triangle(vs[5], vs[6], vs[2], material_));

    //back
    triangles.push_back(new Triangle(vs[5], vs[4], vs[7], material_));
    triangles.push_back(new Triangle(vs[5], vs[7], vs[6], material_));

    // top
    triangles.push_back(new Triangle(vs[3], vs[2], vs[7], material_));
    triangles.push_back(new Triangle(vs[2], vs[6], vs[7], material_));

    // bottom
    triangles.push_back(new Triangle(vs[5], vs[1], vs[0], material_));
    triangles.push_back(new Triangle(vs[5], vs[0], vs[4], material_));

}