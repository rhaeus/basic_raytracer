#include "CornellBoxScene.h"

#include "Sphere.h"
#include "Triangle.h"
#include "Flat.h"
#include "Shiny.h"
#include "Glass.h"
#include "Mirror.h"
#include "PointLight.h"
#include "Cube.h"

CornellBoxScene::CornellBoxScene(int width, int height)
    : Scene(width, height)
{
    float bottom = -height/2.0f;
    float left = -width/2.0f;
    float right = width/2.0f;
    float top = height/2.0f;
    float back = width; //1080
    float front = -600;

    // Camera
    camera = Camera(glm::vec3(0,0,-500), glm::vec3(0,1,0), glm::vec3(0,0,500), 60, width, height);

    // Lighting
    Light* l1 = new PointLight(glm::vec3(0,top - 100, back/2.0f), glm::vec3(1,1,1), 1.0f);
    lights.push_back(std::shared_ptr<Light>(l1));

    // Objects

    std::shared_ptr<Material> m1 = std::make_shared<Material>(Shiny(500, 0.1f));
    m1->setColorMap("../res/textures/2k_earth_daymap.jpg");
    float rsph1 = 200;
    Sphere* sph1 = new Sphere(glm::vec3(200,top - abs(bottom), 880), rsph1, m1);
    objects.push_back(std::shared_ptr<Renderable>(sph1));

    std::shared_ptr<Material> m2 = std::make_shared<Material>(Flat());
    m2->setColorMap("../res/textures/2k_moon.jpg");
    float rsph2 = 100;
    Sphere* sph2 = new Sphere(glm::vec3(-300,200,(back)*2.0f/3.0f), rsph2, m2);
    objects.push_back(std::shared_ptr<Renderable>(sph2));

    std::shared_ptr<Material> m3 = std::make_shared<Material>(Glass(glm::vec3(0,0,0), 1.5f, 500));
    float rsph3 = 50;
    Sphere* sph3 = new Sphere(glm::vec3(left + 500,bottom + 400, 150 ), rsph3, m3);
    objects.push_back(std::shared_ptr<Renderable>(sph3));

    std::shared_ptr<Material> m4 = std::make_shared<Material>(Shiny(glm::vec3(116/255.0f, 0, 158/255.0f), 500, 0.01f));
    float rsph4 = 100;
    Sphere* sph4 = new Sphere(glm::vec3(right - rsph4, bottom + rsph4, 550), rsph4, m4);
    objects.push_back(std::shared_ptr<Renderable>(sph4));

    std::shared_ptr<Material> m5 = std::make_shared<Material>(Flat(glm::vec3(237/255.0f, 162/255.0f, 33/255.0f)));
    float rsph5 = 50;
    Sphere* sph5 = new Sphere(glm::vec3(right - 250,bottom + rsph5, 550), rsph5, m5);
    objects.push_back(std::shared_ptr<Renderable>(sph5));


    std::shared_ptr<Material> m6 = std::make_shared<Material>(Flat());
    m6->setColorMap("../res/textures/rainbowdice.jpg");
    Cube* cube1 = new Cube(glm::vec3(left + 200, bottom + 250, back - 200), m6, glm::vec3(250,250,250), 45, 35.264, 0);
    for (auto t : cube1->getTriangles()) {
        objects.push_back(std::shared_ptr<Renderable>(t));
    }
    

    // cornell box
    std::shared_ptr<Material> floor_m = std::make_shared<Material>(Flat(glm::vec3(0.5,0.5,0.5)));
    Triangle* floor_t1 = new Triangle(glm::vec3(left,bottom+0,front), glm::vec3(right,bottom+0,front), glm::vec3(left, bottom+0, back), floor_m);
    Triangle* floor_t2 = new Triangle(glm::vec3(right,bottom+0,front), glm::vec3(right,bottom+0,back), glm::vec3(left, bottom+0, back), floor_m);
    objects.push_back(std::shared_ptr<Renderable>(floor_t1));
    objects.push_back(std::shared_ptr<Renderable>(floor_t2));

    std::shared_ptr<Material> left_m = std::make_shared<Material>(Flat(glm::vec3(1,0,0)));
    Triangle* left_t1 = new Triangle(glm::vec3(left+0,bottom,front), glm::vec3(left+0,bottom,back), glm::vec3(left+0, top, back), left_m);
    Triangle* left_t2 = new Triangle(glm::vec3(left+0,bottom,front), glm::vec3(left+0,top,back), glm::vec3(left+0, top, front), left_m);
    objects.push_back(std::shared_ptr<Renderable>(left_t1));
    objects.push_back(std::shared_ptr<Renderable>(left_t2));

    std::shared_ptr<Material> right_m = std::make_shared<Material>(Flat(glm::vec3(0,1,0)));
    Triangle* right_t1 = new Triangle(glm::vec3(right-0,bottom,front), glm::vec3(right-0,top,back), glm::vec3(right-0, bottom, back), right_m);
    Triangle* right_t2 = new Triangle(glm::vec3(right-0,bottom,front), glm::vec3(right-0,top,front), glm::vec3(right-0, top, back), right_m);
    objects.push_back(std::shared_ptr<Renderable>(right_t1));
    objects.push_back(std::shared_ptr<Renderable>(right_t2));

    std::shared_ptr<Material> back_m = std::make_shared<Material>(Flat(glm::vec3(90/255.0f, 111/255.0f, 219/255.0f)));
    Triangle* back_t1 = new Triangle(glm::vec3(left,bottom,back-0), glm::vec3(right,bottom,back-0), glm::vec3(right, top, back-0), back_m);
    Triangle* back_t2 = new Triangle(glm::vec3(left,bottom,back-0), glm::vec3(right,top,back-0), glm::vec3(left, top, back-0), back_m);
    objects.push_back(std::shared_ptr<Renderable>(back_t1));
    objects.push_back(std::shared_ptr<Renderable>(back_t2));

    std::shared_ptr<Material> top_m = std::make_shared<Material>(Flat(glm::vec3(1,1,1)));
    Triangle* top_t1 = new Triangle(glm::vec3(left,top-0,front), glm::vec3(right,top-0,back), glm::vec3(right, top-0, front), top_m);
    Triangle* top_t2 = new Triangle(glm::vec3(left,top-0,front), glm::vec3(left,top-0,back), glm::vec3(right, top-0, back), top_m);
    objects.push_back(std::shared_ptr<Renderable>(top_t1));
    objects.push_back(std::shared_ptr<Renderable>(top_t2));

    std::shared_ptr<Material> front_m = std::make_shared<Material>(Flat(glm::vec3(1,0,1)));
    Triangle* front_t1 = new Triangle(glm::vec3(right,bottom,front+0), glm::vec3(left,bottom,front+0), glm::vec3(right, top, front+0), front_m);
    Triangle* front_t2 = new Triangle(glm::vec3(right,top,front+0), glm::vec3(left,bottom,front+0), glm::vec3(left, top, front+0), front_m);
    objects.push_back(std::shared_ptr<Renderable>(front_t1));
    objects.push_back(std::shared_ptr<Renderable>(front_t2));
}