#include "Scene.h"

#include "Sphere.h"
#include "Triangle.h"
#include "Flat.h"
#include "Shiny.h"
#include "Glass.h"
#include "Mirror.h"
#include "PointLight.h"
#include "Texture.h"

Scene::Scene()
{

    Light* l1 = new PointLight(glm::vec3(900,100,0), glm::vec3(1,1,1), 1.0f);
    lights.push_back(std::shared_ptr<Light>(l1));

    // Material* m1 = new Shiny(glm::vec3(158/255.0f, 240/255.0f, 180/255.0f), 1000, 1.0f);
    // Material* m1 = new Shiny(glm::vec3(0,0,0), -1.0f, 0.8f);
    // Material* m1 = new Flat(glm::vec3(0,1,0));
    Material* m1 = new Texture("../res/textures/2k_earth_daymap.jpg");
    // Material* m1 = new Texture("../res/textures/2k_moon.jpg");
    // Material* m1 = new Mirror(1.0f);
    // Material* m1 = new Glass(glm::vec3(0,0,0), 1.5f, 50.0f);
    Sphere* sph1 = new Sphere(glm::vec3(0,0,1500), 600, std::shared_ptr<Material>(m1));
    objects.push_back(std::shared_ptr<Renderable>(sph1));

    // Material* m2 = new Glass(glm::vec3(0,0,0), 1.5f, 100);
    Material* m2 = new Texture("../res/textures/2k_moon.jpg");
    // Material* m2 = new Shiny(glm::vec3(0.6,0.2,0.9), 125, 0.9f);
    // Material* m2 = new Flat(glm::vec3(0,0,1));
    Sphere* sph2 = new Sphere(glm::vec3(-800,0,1200), 110, std::shared_ptr<Material>(m2));
    objects.push_back(std::shared_ptr<Renderable>(sph2));

    // Material* m3 = new Shiny(glm::vec3(0,0,1), 50);
    Material* m3 = new Flat(glm::vec3(0,0,1));
    // Material* m3 = new Glass(glm::vec3(0,0,0), 1.5f, 50.0f);
    Sphere* sph3 = new Sphere(glm::vec3(0,0,400), 50, std::shared_ptr<Material>(m3));
    // objects.push_back(std::shared_ptr<Renderable>(sph3));

    int bottom = -1000;
    int left = -1000;
    int right = 1000;
    int top = 1000;
    int back = 2000;
    int front = -1;

    // cornell box
    Material* floor_m = new Flat(glm::vec3(1,1,1));
    Triangle* floor_t1 = new Triangle(glm::vec3(left,bottom,front), glm::vec3(right,bottom,front), glm::vec3(left, bottom, back), std::shared_ptr<Material>(floor_m));
    Triangle* floor_t2 = new Triangle(glm::vec3(right,bottom,front), glm::vec3(right,bottom,back), glm::vec3(left, bottom, back), std::shared_ptr<Material>(floor_m));
    // objects.push_back(std::shared_ptr<Renderable>(floor_t1));
    // objects.push_back(std::shared_ptr<Renderable>(floor_t2));

    Material* left_m = new Flat(glm::vec3(1,0,0));
    // Material* left_m = new Shiny(glm::vec3(0.6,0.5,0.9), 1000, 0.9f);
    Triangle* left_t1 = new Triangle(glm::vec3(left,bottom,front), glm::vec3(left+500,bottom,back), glm::vec3(left+500, top, back), std::shared_ptr<Material>(left_m));
    Triangle* left_t2 = new Triangle(glm::vec3(left,bottom,front), glm::vec3(left+500,top,back), glm::vec3(left, top, front), std::shared_ptr<Material>(left_m));
    // objects.push_back(std::shared_ptr<Renderable>(left_t1));
    // objects.push_back(std::shared_ptr<Renderable>(left_t2));

    Material* right_m = new Flat(glm::vec3(0,1,0));
    Triangle* right_t1 = new Triangle(glm::vec3(right,bottom,front), glm::vec3(right,top,back), glm::vec3(right, bottom, back), std::shared_ptr<Material>(right_m));
    Triangle* right_t2 = new Triangle(glm::vec3(right,bottom,front), glm::vec3(right,top,front), glm::vec3(right, top, back), std::shared_ptr<Material>(right_m));
    // objects.push_back(std::shared_ptr<Renderable>(right_t1));
    // objects.push_back(std::shared_ptr<Renderable>(right_t2));

    Material* back_m = new Flat(glm::vec3(90/255.0f, 111/255.0f, 219/255.0f));
    // Material* back_m = new Shiny(glm::vec3(0.6,0.5,0.9), -1, 0.9f);
    Triangle* back_t1 = new Triangle(glm::vec3(left,bottom,back), glm::vec3(right,bottom,back-0), glm::vec3(right, top, back-0), std::shared_ptr<Material>(back_m));
    Triangle* back_t2 = new Triangle(glm::vec3(left,bottom,back), glm::vec3(right,top,back-0), glm::vec3(left, top, back), std::shared_ptr<Material>(back_m));
    // objects.push_back(std::shared_ptr<Renderable>(back_t1));
    // objects.push_back(std::shared_ptr<Renderable>(back_t2));

    Material* top_m = new Flat(glm::vec3(1,1,1));
    Triangle* top_t1 = new Triangle(glm::vec3(left,top,front), glm::vec3(right,top,back), glm::vec3(right, top, front), std::shared_ptr<Material>(top_m));
    Triangle* top_t2 = new Triangle(glm::vec3(left,top,front), glm::vec3(left,top,back), glm::vec3(right, top, back), std::shared_ptr<Material>(top_m));
    // objects.push_back(std::shared_ptr<Renderable>(top_t1));
    // objects.push_back(std::shared_ptr<Renderable>(top_t2));

    Material* front_m = new Flat(glm::vec3(1,0,1));
    Triangle* front_t1 = new Triangle(glm::vec3(right,bottom,front), glm::vec3(left,bottom,front), glm::vec3(right, top, front), std::shared_ptr<Material>(front_m));
    Triangle* front_t2 = new Triangle(glm::vec3(right,top,front), glm::vec3(left,bottom,front), glm::vec3(left, top, front), std::shared_ptr<Material>(front_m));
    // objects.push_back(std::shared_ptr<Renderable>(front_t1));
    // objects.push_back(std::shared_ptr<Renderable>(front_t2));

    
}

// Scene::Scene()
// {

//     Light* l1 = new PointLight(glm::vec3(-1000,1000,100), glm::vec3(1,1,1), 1.0f);
//     lights.push_back(std::shared_ptr<Light>(l1));

//     Material* m1 = new Shiny(glm::vec3(158/255.0f, 240/255.0f, 180/255.0f), 1000, 1.0f);
//     // Material* m1 = new Shiny(glm::vec3(0,0,0), -1.0f, 0.8f);
//     // Material* m1 = new Flat(glm::vec3(0,1,0));
//     // Material* m1 = new Mirror(0.9f);
//     // Material* m1 = new Glass(glm::vec3(0,0,0), 1.5f, 50.0f);
//     Sphere* sph1 = new Sphere(glm::vec3(200,0,400), 100, std::shared_ptr<Material>(m1));
//     objects.push_back(std::shared_ptr<Renderable>(sph1));

//     Material* m2 = new Glass(glm::vec3(0,0,0), 1.5f, 50.0f);
//     // Material* m2 = new Shiny(glm::vec3(0.6,0.2,0.9), 125, 0.9f);
//     // Material* m2 = new Flat(glm::vec3(0,0,1));
//     Sphere* sph2 = new Sphere(glm::vec3(150,0,250), 20, std::shared_ptr<Material>(m2));
//     objects.push_back(std::shared_ptr<Renderable>(sph2));

//     // Material* m3 = new Shiny(glm::vec3(0,0,1), 50);
//     Material* m3 = new Flat(glm::vec3(0,0,1));
//     // Material* m3 = new Glass(glm::vec3(0,0,0), 1.5f, 50.0f);
//     Sphere* sph3 = new Sphere(glm::vec3(0,0,400), 50, std::shared_ptr<Material>(m3));
//     objects.push_back(std::shared_ptr<Renderable>(sph3));

//     int bottom = -1000;
//     int left = -1000;
//     int right = 1000;
//     int top = 1000;
//     int back = 1000;
//     int front = -1;

//     // cornell box
//     Material* floor_m = new Flat(glm::vec3(1,1,1));
//     Triangle* floor_t1 = new Triangle(glm::vec3(left,bottom,front), glm::vec3(right,bottom,front), glm::vec3(left, bottom, back), std::shared_ptr<Material>(floor_m));
//     Triangle* floor_t2 = new Triangle(glm::vec3(right,bottom,front), glm::vec3(right,bottom,back), glm::vec3(left, bottom, back), std::shared_ptr<Material>(floor_m));
//     // objects.push_back(std::shared_ptr<Renderable>(floor_t1));
//     // objects.push_back(std::shared_ptr<Renderable>(floor_t2));

//     // Material* left_m = new Flat(glm::vec3(1,0,0));
//     Material* left_m = new Shiny(glm::vec3(0.6,0.5,0.9), 1000, 0.9f);
//     Triangle* left_t1 = new Triangle(glm::vec3(left,bottom,front), glm::vec3(left+500,bottom,back), glm::vec3(left+500, top, back), std::shared_ptr<Material>(left_m));
//     Triangle* left_t2 = new Triangle(glm::vec3(left,bottom,front), glm::vec3(left+500,top,back), glm::vec3(left, top, front), std::shared_ptr<Material>(left_m));
//     // objects.push_back(std::shared_ptr<Renderable>(left_t1));
//     // objects.push_back(std::shared_ptr<Renderable>(left_t2));

//     Material* right_m = new Flat(glm::vec3(0,1,0));
//     Triangle* right_t1 = new Triangle(glm::vec3(right,bottom,front), glm::vec3(right,top,back), glm::vec3(right, bottom, back), std::shared_ptr<Material>(right_m));
//     Triangle* right_t2 = new Triangle(glm::vec3(right,bottom,front), glm::vec3(right,top,front), glm::vec3(right, top, back), std::shared_ptr<Material>(right_m));
//     // objects.push_back(std::shared_ptr<Renderable>(right_t1));
//     // objects.push_back(std::shared_ptr<Renderable>(right_t2));

//     // Material* back_m = new Flat(glm::vec3(1,1,1));
//     Material* back_m = new Shiny(glm::vec3(0.6,0.5,0.9), -1, 0.9f);
//     Triangle* back_t1 = new Triangle(glm::vec3(left,bottom,back), glm::vec3(right,bottom,back-0), glm::vec3(right, top, back-0), std::shared_ptr<Material>(back_m));
//     Triangle* back_t2 = new Triangle(glm::vec3(left,bottom,back), glm::vec3(right,top,back-0), glm::vec3(left, top, back), std::shared_ptr<Material>(back_m));
//     // objects.push_back(std::shared_ptr<Renderable>(back_t1));
//     // objects.push_back(std::shared_ptr<Renderable>(back_t2));

//     Material* top_m = new Flat(glm::vec3(1,1,1));
//     Triangle* top_t1 = new Triangle(glm::vec3(left,top,front), glm::vec3(right,top,back), glm::vec3(right, top, front), std::shared_ptr<Material>(top_m));
//     Triangle* top_t2 = new Triangle(glm::vec3(left,top,front), glm::vec3(left,top,back), glm::vec3(right, top, back), std::shared_ptr<Material>(top_m));
//     // objects.push_back(std::shared_ptr<Renderable>(top_t1));
//     // objects.push_back(std::shared_ptr<Renderable>(top_t2));

//     Material* front_m = new Flat(glm::vec3(1,0,1));
//     Triangle* front_t1 = new Triangle(glm::vec3(right,bottom,front), glm::vec3(left,bottom,front), glm::vec3(right, top, front), std::shared_ptr<Material>(front_m));
//     Triangle* front_t2 = new Triangle(glm::vec3(right,top,front), glm::vec3(left,bottom,front), glm::vec3(left, top, front), std::shared_ptr<Material>(front_m));
//     // objects.push_back(std::shared_ptr<Renderable>(front_t1));
//     // objects.push_back(std::shared_ptr<Renderable>(front_t2));

    
// }

std::vector<std::shared_ptr<Renderable>>& Scene::getObjects() 
{
    return objects;
}

std::vector<std::shared_ptr<Light>>& Scene::getLights() 
{
    return lights;
}