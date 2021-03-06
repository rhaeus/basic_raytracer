#pragma once

#include "Camera.h"
#include "Scene.h"
#include "Ray.h"
#include "BVH.h"

#include <memory>

extern unsigned long long rayTriangleIntersectionTest;
extern unsigned long long rayTriangleIntersections;
extern unsigned long long raySphereIntersectionTest;
extern unsigned long long raySphereIntersections;
extern unsigned long long rayCount;

class Raytracer
{
public:
    Raytracer(int width, int height, std::shared_ptr<Scene> scene, int maxRecursions = 5, int superSamples = 1);
    void trace(glm::vec3* buffer) ;

private:
    glm::vec3 castRay(const Ray& ray) ;
    Intersection getClosestIntersection(const Ray& ray) ;

    glm::vec3 calculateColor(const Intersection& intersection) ;
    glm::vec3 calculateAmbientLighting(const Intersection& intersection,  const glm::vec3& objectColor) ;
    glm::vec3 calculateDiffuseAndSpecularLighting(const Intersection& intersection, const glm::vec3& objectColor) ;
    glm::vec3 calculateReflectiveRefractiveLighting(const Intersection& intersection) ;
    bool isInShadow(const glm::vec3& point, const glm::vec3& lightPosition) ;

    glm::vec3 reflect(const glm::vec3& in, const glm::vec3& normal);
    glm::vec3 refract(const glm::vec3& in, const glm::vec3& normal, float n1, float n2) ;
    void fresnel(const glm::vec3& in, const glm::vec3& normal, float n1, float n2, float& r, float& t) ;


    glm::vec3 castRayAtPixel(int x, int y);

    int width;
    int height;
    // Camera camera;
    std::shared_ptr<Scene> scene;
    int maxRecursions;
    int superSamples;

    float minX;
    float minY;
    float maxX;
    float maxY;

    std::shared_ptr<BVH> bvh;
};