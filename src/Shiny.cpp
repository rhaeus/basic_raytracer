#include "Shiny.h"

Shiny::Shiny(glm::vec3 color_, float shininess_, float reflectiveness_) 
{
    color = color_;
    shininess = shininess_;
    reflectiveness = reflectiveness_;
}

Shiny::Shiny(float shininess, float reflectiveness)
    : Shiny(glm::vec3(0,0,0), shininess, reflectiveness)
{

}
   

