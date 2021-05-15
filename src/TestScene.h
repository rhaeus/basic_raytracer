#include "Scene.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Flat.h"
#include "Shiny.h"
#include "Glass.h"
#include "Mirror.h"
#include "PointLight.h"
#include "Cube.h"

class TestScene : public Scene
{
public:
    TestScene(int width, int height) 
        : Scene(width, height)
    {
        float bottom = -height/2.0f;
        float left = -width/2.0f;
        float right = width/2.0f;
        float top = height/2.0f;
        float back = width;
        float front = 0;

        // Camera
        camera = Camera(glm::vec3(0,0,-500), glm::vec3(0,1,0), glm::vec3(0,0,500), 60, width, height);

        // Lighting
        Light* l1 = new PointLight(glm::vec3(1000,1000, 0), glm::vec3(1,1,1), 1.0f);
        lights.push_back(std::shared_ptr<Light>(l1));

        // Objects

        std::shared_ptr<Material> m1 = std::make_shared<Material>(Flat(glm::vec3(0,1,0)));
        float rsph1 = 400;
        Sphere* sph1 = new Sphere(glm::vec3(0,top - abs(bottom), back - front - rsph1), rsph1, m1);
        objects.push_back(std::shared_ptr<Renderable>(sph1));


        // std::shared_ptr<Material> m2 = std::make_shared<Material>(Glass(glm::vec3(0,0,0), 2.5f, 50));
        std::shared_ptr<Material> m2 = std::make_shared<Material>(Flat(glm::vec3(0,1,0)));
        float rsph2 = 50;
        Sphere* sph2 = new Sphere(glm::vec3(0,top - abs(bottom),100), rsph2, m2);
        objects.push_back(std::shared_ptr<Renderable>(sph2));

    }
};