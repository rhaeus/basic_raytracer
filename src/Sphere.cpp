#include "Sphere.h"
#include <iostream>
#include <math.h>

glm::vec2 Sphere::getUV(const glm::vec3& pos) {
    glm::vec3 d = glm::normalize(center - pos);

    float u = 0.5f - atan2(d.x, d.z) / (2.0f * M_PI);
    float v = 0.5f + asin(d.y) / M_PI;

    return glm::vec2(u, v);
}

glm::vec3 Sphere::getColor(glm::vec3 pos)  
{
    return material->getColor(pos, getUV(pos)); 
}

glm::vec3 Sphere::getNormal(glm::vec3 pos)  
{
    glm::vec3 normal = glm::normalize(pos- center);
    return material->getNormal(normal, pos, getUV(pos));
}

Sphere::Sphere(glm::vec3 center_, float radius_, std::shared_ptr<Material> material_) 
    : Renderable(material_),
    center(center_),
    radius(radius_) 
{
    bounds = std::make_shared<AABB>(center - radius, center + radius);
}

Intersection Sphere::intersect(const Ray& ray)
{
    raySphereIntersectionTest++;

    float lambda = -1;

    glm::vec3 p_r = ray.getOrigin();
    glm::vec3 t_r = ray.getDirection();
    glm::vec3 c = center;
    float r = radius;

    float p_half = glm::dot(t_r, p_r - c) / glm::dot(t_r, t_r);
    float q = (glm::dot(p_r - c, p_r - c) - r*r) / glm::dot(t_r, t_r);

    // does solution exist
    float root = p_half*p_half - q;
    if (root < 0) {
        return Intersection(ray); // no intersection
    }

    float l1 = -p_half + sqrt(root);
    float l2 = -p_half - sqrt(root);

    // if ( (l1 > maxLambda && l2 > maxLambda) || (l1 < 0 && l2 < 0) ) {
    if (l1 < 0 && l2 < 0) {
        return Intersection(ray);
    } else {
        if (l1 < 0) {
            lambda = l2;
        } else if (l2 < 0) {
            lambda = l1;
        } else {
            if (l1 < l2) {
                lambda = l1;
            } else {
                lambda = l2;
            }
        }
    }

    glm::vec3 point = ray.getOrigin() + (ray.getDirection() * lambda);
    glm::vec3 normal = getNormal(point);

    // Normal needs to be flipped if this is a refractive ray.
    if (glm::dot(ray.getDirection(), normal) > 0) {
        normal = normal * -1.0f;
    }

    raySphereIntersections++;
    return Intersection (ray, lambda, point, normal, shared_from_this(), true);
    
}
