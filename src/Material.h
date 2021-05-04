#pragma once

#include <glm/glm.hpp>

class Material
{
public:
    Material()
        : color(glm::vec3(0,0,0)), shininess(-1.0f), reflectiveness(-1.0f), ior(-1.0f) {}
    

    virtual glm::vec3 getColor(glm::vec2 pos)  {return color; };

    virtual glm::vec3 getNormal(glm::vec2 pos)  { return glm::vec3(0,0,0); };
    virtual float getShininess()  { return shininess; /*not shiny*/};
    virtual float getReflectiveness()  {return reflectiveness; /* not reflective*/};
    virtual float getIndexOfRefraction()  { return ior; };
    

protected:
    int* normalMap;
    int* colorMap;
    glm::vec3 color;
    float shininess;
    float reflectiveness;
    float ior;

};