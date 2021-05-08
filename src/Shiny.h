#include "Material.h"

class Shiny : public Material
{
public:
   Shiny(glm::vec3 color, float shininess, float reflectiveness);
   Shiny(float shininess, float reflectiveness);

};