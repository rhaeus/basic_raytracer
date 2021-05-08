#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>


Texture::Texture(std::string filePath_) 
{
    // stbi_set_flip_vertically_on_load(1);
    textureData = stbi_load(filePath_.c_str(), &width, &height, &channels, 0);
    if(textureData == NULL) {
        std::cerr << "Error while loading texture from " << filePath_ << std::endl;
    }

}

Texture::~Texture() 
{
    stbi_image_free(textureData); 
}

glm::vec3 Texture::getColor(glm::vec2 pos) 
{
    // TODO filter
    // pos is in [0,1]
    // scale u and v to image width and height

    int u = pos.x * width;
    int v = pos.y * height;

    unsigned bytePerPixel = channels;
    unsigned char* pixelOffset = textureData + (u + width * v) * bytePerPixel;
    unsigned char r = pixelOffset[0];
    unsigned char g = pixelOffset[1];
    unsigned char b = pixelOffset[2];
    unsigned char a = channels >= 4 ? pixelOffset[3] : 0xff;

    glm::vec3 color;
    color.r = r / 255.0f;
    color.g = g / 255.0f;
    color.b = b / 255.0f;

    return color;
}
   
