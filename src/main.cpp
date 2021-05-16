#include <iostream>
#include <glm/glm.hpp>
#include <fstream>

#include "Raytracer.h"
#include "CornellBoxScene.h"
#include "TestScene.h"
#include "MaterialShowCase.h"

unsigned long long rayTriangleIntersectionTest = 0;
unsigned long long rayTriangleIntersections = 0;
unsigned long long raySphereIntersectionTest = 0;
unsigned long long raySphereIntersections = 0;
unsigned long long rayAABBIntersectionTest = 0;
unsigned long long bailEarly = 0;
unsigned long long rayCount = 0;

int main(int argc, char** argv) {

    int width = 1200;
    int height = 1200;
    int maxRecursions = 5;
    int superSamples = 4;

    rayCount = 0;
    rayTriangleIntersectionTest = 0;
    rayTriangleIntersections = 0;
    raySphereIntersectionTest = 0;
    raySphereIntersections = 0;
    bailEarly = 0;
    rayCount = 0;


    std::shared_ptr<Scene> scene = std::make_shared<CornellBoxScene>(CornellBoxScene(width, height));
    // std::shared_ptr<Scene> scene = std::make_shared<TestScene>(TestScene(width, height));
    // std::shared_ptr<Scene> scene = std::make_shared<MaterialShowCase>(MaterialShowCase(width, height));

    Raytracer raytracer(width, height, scene, maxRecursions, superSamples);
    glm::vec3* buffer = (glm::vec3*) malloc(width * height * sizeof(glm::vec3));
    std::cout << "Starting rendering" << std::endl;
    clock_t timeStart = clock();
    raytracer.trace(buffer);
    clock_t timeEnd = clock();
    std::cout << "Rendering done" << std::endl;

    std::cout << "Ray count: " << rayCount << std::endl;
    std::cout << "Ray-Triangle Intersection Tests: " << rayTriangleIntersectionTest << std::endl;
    std::cout << "Ray-Triangle Intersections: " << rayTriangleIntersections << std::endl;
    std::cout << "Ray-Sphere Intersection Tests: " << raySphereIntersectionTest << std::endl;
    std::cout << "Ray-Sphere Intersections: " << raySphereIntersections << std::endl;
    std::cout << "Ray-AABB Intersections: " << rayAABBIntersectionTest << std::endl;
    std::cout << "Bail early: " << bailEarly << std::endl;
    
    printf("Render time: %04.2f (sec)\n", (float)(timeEnd - timeStart) / CLOCKS_PER_SEC);

    // save framebuffer to file
    std::ofstream ofs;
    ofs.open("/home/ramona/basic_out.ppm");
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (uint32_t i = 0; i < height * width; ++i) {
        double r = (buffer[i].r > 1.0) ? 1.0 : buffer[i].r;
        double g = (buffer[i].g > 1.0) ? 1.0 : buffer[i].g;
        double b = (buffer[i].b > 1.0) ? 1.0 : buffer[i].b;
        unsigned char rbyte = (unsigned char)(r * 255);
        unsigned char gbyte = (unsigned char)(g * 255);
        unsigned char bbyte = (unsigned char)(b * 255);
        // char r = (char)(255.0f * glm::clamp(buffer[i].x,0.0f, 1.0f));
        // char g = (char)(255.0f * glm::clamp(buffer[i].y,0.0f, 1.0f));
        // char b = (char)(255.0f * glm::clamp(buffer[i].z,0.0f, 1.0f));
        ofs << rbyte << gbyte << bbyte;
    }

    ofs.close();

    free(buffer);

    // std::cout << "done" << std::endl;
    return 0;
}