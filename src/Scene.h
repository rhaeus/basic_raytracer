#pragma once

#include <memory>
#include <vector>

#include "Renderable.h"
#include "Light.h"

class Scene
{
public:
    Scene();

    std::vector<std::shared_ptr<Renderable>>& getObjects() ;
    std::vector<std::shared_ptr<Light>>& getLights() ;


private:
    std::vector<std::shared_ptr<Renderable>> objects;
    std::vector<std::shared_ptr<Light>> lights;

};