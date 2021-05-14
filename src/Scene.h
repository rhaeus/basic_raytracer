#pragma once

#include <memory>
#include <vector>

#include "Renderable.h"
#include "Light.h"
#include "Camera.h"

class Scene
{
public:
    Scene(int width, int height);

    std::vector<std::shared_ptr<Renderable>>& getObjects() ;
    std::vector<std::shared_ptr<Light>>& getLights() ;
    const Camera& getCamera() const;


protected:
    std::vector<std::shared_ptr<Renderable>> objects;
    std::vector<std::shared_ptr<Light>> lights;
    Camera camera;

};