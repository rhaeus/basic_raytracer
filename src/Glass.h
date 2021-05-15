#include "Material.h"

class Glass : public Material
{
public:
   Glass(glm::vec3 color, float ior, float shininess, float density_ = 0.05f);
};