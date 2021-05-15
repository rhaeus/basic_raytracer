#include "Sphere.h"
#include <iostream>
#include <math.h>

glm::vec3 Sphere::getColor(glm::vec3 pos)  
{
    // calculate u and v
    

    glm::vec3 d = glm::normalize(center - pos);

    float u = 0.5f - atan2(d.x, d.z) / (2.0f * M_PI);
    float v = 0.5f + asin(d.y) / M_PI;


    // glm::vec3 n = glm::normalize(pos - center);

    // // will be in [-1, 1]
    // float u = asin(normal.x) / M_PI + 0.5f;
    // float v = asin(normal.y) / M_PI + 0.5f;


    // float u = atan2(n.x, n.z) / (2.0f*M_PI) + 0.5f;
    // float v = n.y * 0.5f + 0.5f;

    return material->getColor(glm::vec2(u, v)); 
}

glm::vec3 Sphere::getNormal(glm::vec3 pos)  
{
    glm::vec3 d = glm::normalize(center - pos);
    float u = 0.5f - atan2(d.x, d.z) / (2.0f * M_PI);
    float v = 0.5f + asin(d.y) / M_PI;

    glm::vec3 n = material->getNormal(glm::vec2(u, v));

    if (n != glm::vec3(0,0,0)) {
         return n;
    }
   

    return -d;
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

    // std::cout << "here" << std::endl;
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

    // glm::vec3 point = ray.getPoint(lambda);
    // glm::vec3 normal = glm::normalize(intersection_point - center);

    // float distance = lambda;
    glm::vec3 point = ray.getOrigin() + (ray.getDirection() * lambda);
    // glm::vec3 normal = glm::normalize(point - center);
    glm::vec3 normal = getNormal(point);

    // Normal needs to be flipped if this is a refractive ray.
    if (glm::dot(ray.getDirection(), normal) > 0) {
        normal = normal * -1.0f;
    }

    raySphereIntersections++;
    return Intersection (ray, lambda, point, normal, shared_from_this(), true);
    
    // intersection = Intersection(intersection_point, lambda, normal, 0, shared_from_this(), ray);
    // std::cout << "interseeeect spheeeere" << std::endl << intersection_point.x << ", " << intersection_point.y << ", " << intersection_point.z << std::endl;
    // std::cout << "ray x " << ray.getOrigin().x << std::endl;
    // std::cout << "ray y " << ray.getOrigin().y << std::endl;
    // std::cout << "ray z " << ray.getOrigin().z << std::endl;

    // std::cout << "ray dir x " << ray.getDirection().x << std::endl;
    // std::cout << "ray dir y " << ray.getDirection().y << std::endl;
    // std::cout << "ray dir z " << ray.getDirection().z << std::endl;
    // return true;

}

// Intersection Sphere::intersect(const Ray& ray) {
//    glm::vec3 deltap = ray.getOrigin() - center;
//    double a = glm::dot(ray.getDirection(), ray.getDirection());
//    double b = glm::dot(deltap, ray.getDirection()) * 2.0f;
//    double c = glm::dot(deltap, deltap) - (radius * radius);

//    double disc = b * b - 4 * a * c;
//    if (disc < 0) {
//       return Intersection(ray); // No intersection.
//    }

//    disc = sqrt(disc);

//    double q;
//    if (b < 0) {
//       q = (-b - disc) * 0.5;
//    } else {
//       q = (-b + disc) * 0.5;
//    }

//    double r1 = q / a;
//    double r2 = c / q;

//    if (r1 > r2) {
//       double tmp = r1;
//       r1 = r2;
//       r2 = tmp;
//    }

//    double distance = r1;
//    if (distance < 0) {
//       distance = r2;
//    }

//    if (distance < 0 || isnan(distance)) {
//       return Intersection(ray); // No intersection.
//    }

//    glm::vec3 point = ray.getOrigin() + (ray.getDirection() * (float)distance);
//    glm::vec3 normal = glm::normalize(point - center);

// //    normal = material->modifyNormal(normal, point);

//    // Normal needs to be flipped if this is a refractive ray.
//    if (glm::dot(ray.getDirection(), normal) > 0) {
//       normal = normal * -1.0f;
//    }


//    return Intersection (ray, distance, point, normal, shared_from_this(), true);
// }