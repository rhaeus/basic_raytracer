#include "MaterialShowCase.h"

#include "Sphere.h"
#include "Triangle.h"
#include "Flat.h"
#include "Shiny.h"
#include "Glass.h"
#include "Mirror.h"
#include "PointLight.h"
#include "Cube.h"
#include "RectangleLight.h"
#include "Plane.h"
#include "Turbulent.h"
#include "Marble.h"
#include "Wood.h"

MaterialShowCase::MaterialShowCase(int width, int height)
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
    // Light* l1 = new PointLight(glm::vec3(0, 0, -10000), glm::vec3(1,1,1), 1.0f);
    Light* l1 = new RectangleLight(glm::vec3(300,300, -80), glm::vec3(50, 0, 0), glm::vec3(0, 50, 0), 10, 10, glm::vec3(1,1,1), 1.0f);
    lights.push_back(std::shared_ptr<Light>(l1));

    // Objects

    // Flat
    // uni
    std::shared_ptr<Material> m1 = std::make_shared<Flat>(Flat(glm::vec3(12/255.0f, 153/255.0f, 204/255.0f)));
    float rsph1 = 100;
    Sphere* sph1 = new Sphere(glm::vec3(left + 300,top - 200, back - rsph1), rsph1, m1);
    objects.push_back(std::shared_ptr<Renderable>(sph1));

    // texture
    std::shared_ptr<Material> m2 = std::make_shared<Flat>(Flat());
    m2->setColorMap("../res/textures/2k_earth_daymap.jpg");
    float rsph2 = 100;
    Sphere* sph2 = new Sphere(glm::vec3(left + 600, top - 200, back - rsph2), rsph2, m2);
    objects.push_back(std::shared_ptr<Renderable>(sph2));

    // bumpy
    std::shared_ptr<Material> m3 = std::make_shared<Flat>(Flat(glm::vec3(191/255.0f, 15/255.0f, 65/255.0f)));
    m3->setBumpiness(2.5, 1.05);
    float rsph3 = 100;
    Sphere* sph3 = new Sphere(glm::vec3(left + 900, top - 200, back - rsph3), rsph3, m3);
    objects.push_back(std::shared_ptr<Renderable>(sph3));

    // Glass
    // red
    std::shared_ptr<Material> m4 = std::make_shared<Glass>(Glass(glm::vec3(0.1,0,0), 1.5f, 500));
    float rsph4 = 100;
    Sphere* sph4 = new Sphere(glm::vec3(left + 300,top - 450, back - rsph4), rsph4, m4);
    objects.push_back(std::shared_ptr<Renderable>(sph4));

    // black
    std::shared_ptr<Material> m5 = std::make_shared<Glass>(Glass(glm::vec3(0,0,0), 1.5f, 500));
    // m5->setColorMap("../res/textures/checkerboard_black_white.png");
    float rsph5 = 100;
    Sphere* sph5 = new Sphere(glm::vec3(left + 600,top - 450, back - rsph5), rsph5, m5);
    objects.push_back(std::shared_ptr<Renderable>(sph5));

    // bumpy
    std::shared_ptr<Material> m6 = std::make_shared<Glass>(Glass(glm::vec3(0,0,0), 1.5f, 5));
    m6->setBumpiness(10, 0.5);
    float rsph6 = 100;
    Sphere* sph6 = new Sphere(glm::vec3(left + 900,top - 450, back - rsph6), rsph6, m6);
    objects.push_back(std::shared_ptr<Renderable>(sph6));

    // shiny
    // uni
    std::shared_ptr<Material> m7 = std::make_shared<Shiny>(Shiny(glm::vec3(232/255.0f, 182/255.0f, 0), 250, 0.3));
    float rsph7 = 100;
    Sphere* sph7 = new Sphere(glm::vec3(left + 300,top - 700, back - rsph7), rsph7, m7);
    objects.push_back(std::shared_ptr<Renderable>(sph7));

    // texture
    std::shared_ptr<Material> m8 = std::make_shared<Shiny>(Shiny(glm::vec3(232/255.0f, 182/255.0f, 0), 250, 0.05));
    m8->setColorMap("../res/textures/2k_moon.jpg");
    float rsph8 = 100;
    Sphere* sph8 = new Sphere(glm::vec3(left + 600, top - 700, back - rsph8), rsph8, m8);
    objects.push_back(std::shared_ptr<Renderable>(sph8));

    // bumpy
    std::shared_ptr<Material> m9 = std::make_shared<Shiny>(Shiny(glm::vec3(0/255.0f, 232/255.0f, 128/255.0f), 50, 0.1));
    m9->setBumpiness(8, 0.5);
    float rsph9 = 100;
    Sphere* sph9 = new Sphere(glm::vec3(left + 900, top - 700, back - rsph9), rsph9, m9);
    objects.push_back(std::shared_ptr<Renderable>(sph9));

    // procedural texture
    // Marble
    std::shared_ptr<Material> m10 = std::make_shared<Marble>(Marble(glm::vec3(0.4, 0.3, 0.2), glm::vec3(0.6, 0.7, 0.8), 0.5, 50, 0.15));
    m10->setBumpiness(0.5, 0.25);
    float rsph10 = 100;
    Sphere* sph10 = new Sphere(glm::vec3(left + 300,top - 950, back - rsph10), rsph10, m10);
    objects.push_back(std::shared_ptr<Renderable>(sph10));

    // Turbulent
    std::shared_ptr<Material> m11 = std::make_shared<Turbulent>(Turbulent(glm::vec3(92/255.0f, 0, 184/255.0f), glm::vec3(0.5, 0.5, 0.5), 0.75));
    float rsph11 = 100;
    Sphere* sph11 = new Sphere(glm::vec3(left + 600, top - 950, back - rsph11), rsph11, m11);
    objects.push_back(std::shared_ptr<Renderable>(sph11));

    // wood
    std::shared_ptr<Material> m12 = std::make_shared<Wood>(Wood(glm::vec3(0.1043, 0.0737, 0.0517), glm::vec3(0.4215, 0.2686, 0.1888), 0.01));
    // m12->setBumpiness(0.6, 0.45);
    float rsph12 = 100;
    Sphere* sph12 = new Sphere(glm::vec3(left + 900, top - 950, back - rsph12), rsph12, m12);
    objects.push_back(std::shared_ptr<Renderable>(sph12));

    std::shared_ptr<Material> back_m = std::make_shared<Material>(Flat(glm::vec3(90/255.0f, 111/255.0f, 219/255.0f)));
    back_m->setColorMap("../res/textures/checkerboard_double.png");
    Plane* back_wall = new Plane(glm::vec3(left,bottom,back), glm::vec3(right + abs(left), 0, 0), glm::vec3(0, abs(bottom)+top, 0), back_m);
    for (auto t : back_wall->getTriangles()) {
        objects.push_back(std::shared_ptr<Renderable>(t));
    }
}