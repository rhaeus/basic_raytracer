#include <iostream>
#include <glm/glm.hpp>
#include <fstream>

#include "Raytracer.h"

int main(int argc, char** argv) {
    int width = 720;
    int height = 720;
    int maxRecursions = 5;

    Camera camera(glm::vec3(0,0,0), glm::vec3(0,1,0), glm::vec3(0,0,5), 90);
    Scene scene;

    Raytracer raytracer(width, height, camera, scene, maxRecursions);
    glm::vec3* buffer = (glm::vec3*) malloc(width * height * sizeof(glm::vec3));
    std::cout << "Starting rendering" << std::endl;
    raytracer.trace(buffer);
    std::cout << "Rendering done" << std::endl;

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

    std::cout << "done" << std::endl;
    return 0;
}