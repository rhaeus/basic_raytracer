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
    float back = width;
    float front = 0;

    // Camera
    camera = Camera(glm::vec3(0,0,-500), glm::vec3(0,1,0), glm::vec3(0,0,500), 60, width, height);

    // Lighting
    Light* l1 = new PointLight(glm::vec3(0,top - 100, (back - front)/2.0f), glm::vec3(1,1,1), 1.0f);
    lights.push_back(std::shared_ptr<Light>(l1));

    // Objects

    std::shared_ptr<Material> m1 = std::make_shared<Material>(Shiny(500, 0.1f));
    m1->setColorMap("../res/textures/2k_earth_daymap.jpg");
    float rsph1 = 200;
    Sphere* sph1 = new Sphere(glm::vec3(200,top - abs(bottom), back - front - rsph1), rsph1, m1);
    objects.push_back(std::shared_ptr<Renderable>(sph1));


    std::shared_ptr<Material> m2 = std::make_shared<Material>(Flat());
    m2->setColorMap("../res/textures/2k_moon.jpg");
    float rsph2 = 100;
    Sphere* sph2 = new Sphere(glm::vec3(-300,200,(back - front)*2.0f/3.0f), rsph2, m2);
    objects.push_back(std::shared_ptr<Renderable>(sph2));

    std::shared_ptr<Material> m3 = std::make_shared<Material>(Flat(glm::vec3(235/255.0f, 176/255.0f, 75/255.0f)));
    float rsph3 = 120;
    Sphere* sph3 = new Sphere(glm::vec3(right - 200,bottom + rsph3, (back - front)/3.0f + 250), rsph3, m3);
    objects.push_back(std::shared_ptr<Renderable>(sph3));


    std::shared_ptr<Material> m4 = std::make_shared<Material>(Flat());
    m4->setColorMap("../res/textures/rainbowdice.jpg");
    Cube* cube1 = new Cube(glm::vec3(left + 200, bottom + 250, back - 200), m4, glm::vec3(250,250,250), 45, 35.264, 0);
    for (auto t : cube1->getTriangles()) {
        objects.push_back(std::shared_ptr<Renderable>(t));
    }
    

    // cornell box
    std::shared_ptr<Material> floor_m = std::make_shared<Material>(Flat(glm::vec3(1,1,1)));
    Triangle* floor_t1 = new Triangle(glm::vec3(left,bottom,front), glm::vec3(right,bottom,front), glm::vec3(left, bottom, back), floor_m);
    Triangle* floor_t2 = new Triangle(glm::vec3(right,bottom,front), glm::vec3(right,bottom,back), glm::vec3(left, bottom, back), floor_m);
    objects.push_back(std::shared_ptr<Renderable>(floor_t1));
    objects.push_back(std::shared_ptr<Renderable>(floor_t2));

    std::shared_ptr<Material> left_m = std::make_shared<Material>(Flat(glm::vec3(1,0,0)));
    Triangle* left_t1 = new Triangle(glm::vec3(left,bottom,front), glm::vec3(left+0,bottom,back), glm::vec3(left+0, top, back), left_m);
    Triangle* left_t2 = new Triangle(glm::vec3(left,bottom,front), glm::vec3(left+0,top,back), glm::vec3(left, top, front), left_m);
    objects.push_back(std::shared_ptr<Renderable>(left_t1));
    objects.push_back(std::shared_ptr<Renderable>(left_t2));

    std::shared_ptr<Material> right_m = std::make_shared<Material>(Flat(glm::vec3(0,1,0)));
    Triangle* right_t1 = new Triangle(glm::vec3(right,bottom,front), glm::vec3(right,top,back), glm::vec3(right, bottom, back), right_m);
    Triangle* right_t2 = new Triangle(glm::vec3(right,bottom,front), glm::vec3(right,top,front), glm::vec3(right, top, back), right_m);
    objects.push_back(std::shared_ptr<Renderable>(right_t1));
    objects.push_back(std::shared_ptr<Renderable>(right_t2));

    std::shared_ptr<Material> back_m = std::make_shared<Material>(Flat(glm::vec3(90/255.0f, 111/255.0f, 219/255.0f)));
    Triangle* back_t1 = new Triangle(glm::vec3(left,bottom,back), glm::vec3(right,bottom,back-0), glm::vec3(right, top, back-0), back_m);
    Triangle* back_t2 = new Triangle(glm::vec3(left,bottom,back), glm::vec3(right,top,back-0), glm::vec3(left, top, back), back_m);
    objects.push_back(std::shared_ptr<Renderable>(back_t1));
    objects.push_back(std::shared_ptr<Renderable>(back_t2));

    std::shared_ptr<Material> top_m = std::make_shared<Material>(Flat(glm::vec3(1,1,1)));
    Triangle* top_t1 = new Triangle(glm::vec3(left,top,front), glm::vec3(right,top,back), glm::vec3(right, top, front), top_m);
    Triangle* top_t2 = new Triangle(glm::vec3(left,top,front), glm::vec3(left,top,back), glm::vec3(right, top, back), top_m);
    objects.push_back(std::shared_ptr<Renderable>(top_t1));
    objects.push_back(std::shared_ptr<Renderable>(top_t2));

    // std::shared_ptr<Material> front_m = std::make_shared<Material>(Flat(glm::vec3(1,0,1)));
    // Triangle* front_t1 = new Triangle(glm::vec3(right,bottom,front), glm::vec3(left,bottom,front), glm::vec3(right, top, front), front_m);
    // Triangle* front_t2 = new Triangle(glm::vec3(right,top,front), glm::vec3(left,bottom,front), glm::vec3(left, top, front), front_m);
    // objects.push_back(std::shared_ptr<Renderable>(front_t1));
    // objects.push_back(std::shared_ptr<Renderable>(front_t2));
}