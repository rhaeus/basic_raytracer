#include "Scene.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Flat.h"
#include "Shiny.h"
#include "Glass.h"
#include "Mirror.h"
#include "PointLight.h"
#include "Cube.h"
#include "Turbulent.h"
#include "Marble.h"
#include "Wood.h"

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
        camera = Camera(glm::vec3(0,0,0), glm::vec3(0,1,0), glm::vec3(0,0,500), 60, width, height);

        // Lighting
        Light* l1 = new PointLight(glm::vec3(0,50, -100), glm::vec3(1,1,1), 1.0f);
        lights.push_back(std::shared_ptr<Light>(l1));

        // Objects

        // std::shared_ptr<Material> m1 = std::make_shared<Turbulent>(Turbulent(glm::vec3(0.571, 0.527, 0.125), glm::vec3(0.68, 0.081, 0.994), 10));
        // std::shared_ptr<Material> m1 = std::make_shared<Turbulent>(Turbulent(glm::vec3(1,1,1), glm::vec3(0,0,0), 20));
        // std::shared_ptr<Material> m1 = std::make_shared<Marble>(Marble(glm::vec3(0,1,0), glm::vec3(1,0,0), 5, 500, 1));
        // std::shared_ptr<Material> m1 = std::make_shared<Wood>(Wood(glm::vec3( 0.1043, 0.0737, 0.0517), glm::vec3(0.4215, 0.2686, 0.1888), 0.1));

        std::shared_ptr<Material> m1 = std::make_shared<Shiny>(Shiny(glm::vec3(1,0,0), 5, 0.5));
        m1->setBumpiness(0.5, 0.25);
         
        // std::shared_ptr<Material> m1 = std::make_shared<Flat>(Flat(glm::vec3(0.571, 0.527, 0.125)));
        float rsph1 = 125;
        Sphere* sph1 = new Sphere(glm::vec3(0,0, 250), rsph1, m1);
        objects.push_back(std::shared_ptr<Renderable>(sph1));


        // std::shared_ptr<Material> m2 = std::make_shared<Material>(Glass(glm::vec3(0,0,0), 2.5f, 50));
        // std::shared_ptr<Material> m2 = std::make_shared<Material>(Flat(glm::vec3(0,1,0)));
        // float rsph2 = 50;
        // Sphere* sph2 = new Sphere(glm::vec3(0,top - abs(bottom),100), rsph2, m2);
        // objects.push_back(std::shared_ptr<Renderable>(sph2));

    }
};