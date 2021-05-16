#include "Material.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

Material::Material()
    : color(glm::vec3(0,0,0)), shininess(-1.0f), reflectiveness(-1.0f), ior(-1.0f), density(1.0f)
{
    colorMap = nullptr;
    normalMap = nullptr;
    isBumpy = false;
}

void Material::setColorMap(std::string file)
{
    if (colorMap)
        stbi_image_free(colorMap); 

    loadMap(file, &colorMap, widthColorMap, heightColorMap, channelsColorMap);
}
// void Material::setNormalMap(std::string file) 
// {
//     if (normalMap)
//         stbi_image_free(normalMap); 

//     loadMap(file, &normalMap, widthNormalMap, heightNormalMap, channelsNormalMap);
// }

void Material::loadMap(std::string file, unsigned char** target, int& width, int& height, int& channels) 
{
    *target = NULL;
    *target = stbi_load(file.c_str(), &width, &height, &channels, 0);
    if (target == NULL) {
        std::cout << "Error while loading " << file << std::endl;
    }
}

Material::~Material() 
{
    if (colorMap)
        stbi_image_free(colorMap); 
    if (normalMap)
        stbi_image_free(normalMap); 
}

glm::vec3 Material::mapLookup(glm::vec2 pos, unsigned char* target, int& width, int& height, int& channels)
{
    glm::vec3 result;
    // TODO filter
    // pos is in [0,1]
    // scale u and v to image width and height

    // int u = pos.x * width/2.0f + width/2.0f;
    // int v = pos.y * height/2.0f + height/2.0f;

    int u = pos.s * width;
    int v = pos.t * height;

    unsigned bytePerPixel = channels;
    unsigned char* pixelOffset = target + (u + width * v) * bytePerPixel;
    unsigned char r = pixelOffset[0];
    unsigned char g = pixelOffset[1];
    unsigned char b = pixelOffset[2];
    unsigned char a = channels >= 4 ? pixelOffset[3] : 0xff;

    result.r = r / 255.0f;
    result.g = g / 255.0f;
    result.b = b / 255.0f;

    return result;
}

glm::vec3 Material::getColor(const glm::vec3& pos, const glm::vec2& uv) {
    glm::vec3 col = color;
    if (colorMap) 
    {
       col = mapLookup(uv, colorMap, widthColorMap, heightColorMap, channelsColorMap);
    }

    return col;
}

void Material::setBumpiness(float scale, float amount) 
{
    isBumpy = true;
    bumpScale = scale;
    bumpAmount = amount;
}

glm::vec3 Material::getNormal(const glm::vec3& objectNormal, const glm::vec3& pos, const glm::vec2& uv) {
    if (isBumpy) {
        glm::vec3 noise(0,0,0);
        double x = pos.x / bumpScale;
        double y = pos.y / bumpScale;
        double z = pos.z / bumpScale;

        noise.x = (float)(perlinNoise.noise(x, y, z));
        noise.y = (float)(perlinNoise.noise(y, z, x));
        noise.z = (float)(perlinNoise.noise(z, x, y));

        return glm::normalize(objectNormal + noise * bumpAmount);
    } else {
        return objectNormal;
    }
}