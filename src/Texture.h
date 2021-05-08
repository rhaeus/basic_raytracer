#include "Material.h"
#include <string>

class Texture : public Material
{
public:
   Texture(std::string filePath);
   ~Texture();
   glm::vec3 getColor(glm::vec2 pos)  override;

private:
   int width, height, channels;
   unsigned char *textureData;
   
};