#pragma once

#include <glm/glm.hpp>
#include <string>

class Material
{
public:
    Material();
    ~Material();

    virtual glm::vec3 getColor(glm::vec3 pos, glm::vec2 uv);
    virtual glm::vec3 getNormal(glm::vec2 pos);

    void setColorMap(std::string file);
    void setNormalMap(std::string file);


    virtual float getShininess()  { return shininess; /*not shiny*/};
    virtual float getReflectiveness()  {return reflectiveness; /* not reflective*/};
    virtual float getIndexOfRefraction()  { return ior; };
    virtual float getDensity()  { return density; /*not shiny*/};
    

protected:
    void loadMap(std::string file, unsigned char** target, int& width, int& height, int& channels);
    glm::vec3 mapLookup(glm::vec2 pos, unsigned char* target, int& width, int& height, int& channels);

    int widthColorMap, heightColorMap, channelsColorMap;
    unsigned char *colorMap;

    int widthNormalMap, heightNormalMap, channelsNormalMap;
    unsigned char *normalMap;


    glm::vec3 color;
    float shininess;
    float reflectiveness;
    float ior;
    float density;

};