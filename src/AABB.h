#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Ray.h"
extern unsigned long long rayAABBIntersectionTest;

class Face 
{
public:
    Face(glm::vec3 lowerLeft_, glm::vec3 u_, glm::vec3 v_)
        : lowerLeft(lowerLeft_), u(u_), v(v_) {};

    bool intersects(const Ray& ray) {
        const glm::vec3 b = ray.getOrigin() - lowerLeft;
        glm::mat3 A(-ray.getDirection(), u, v);
        glm::vec3 x = glm::inverse(A) * b; // x = (t u v)^T

        float t = x.x;
        float u = x.y;
        float v = x.z;

        return u >= 0 && v >= 0 && u <= 1 && v <= 1 && t > 0;
    }

private:
    glm::vec3 lowerLeft; 
    glm::vec3 u; 
    glm::vec3 v;
};

class AABB
{
public:
    // AABB() 
    //     : AABB(glm::vec3(std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max()), 
    //     glm::vec3(std::numeric_limits<float>::min(),std::numeric_limits<float>::min(),std::numeric_limits<float>::min())) {};
    AABB(glm::vec3 min_, glm::vec3 max_);
    bool intersects(const Ray& ray) const;
    glm::vec3 getCenter();
    const glm::vec3& getMin() { return min; };
    const glm::vec3& getMax() { return max; };

    // Overload + operator to add two aabb objects.
    AABB operator+(const AABB& other) {
        // calculate box that encloses both bounds
        glm::vec3 enclosingMin(0,0,0);
        glm::vec3 enclosingMax(0,0,0);

        if (this->min.x < other.min.x)
            enclosingMin.x = this->min.x;
        else
            enclosingMin.x = other.min.x;

        if (this->min.y < other.min.y)
            enclosingMin.y = this->min.y;
        else
            enclosingMin.y = other.min.y;
        
        if (this->min.z < other.min.z)
            enclosingMin.z = this->min.z;
        else
            enclosingMin.z = other.min.z;


        if (this->max.x > other.max.x)
            enclosingMax.x = this->max.x;
        else
            enclosingMax.x = other.max.x;

        if (this->max.y > other.max.y)
            enclosingMax.y = this->max.y;
        else
            enclosingMax.y = other.max.y;
        
        if (this->max.z > other.max.z)
            enclosingMax.z = this->max.z;
        else
            enclosingMax.z = other.max.z;

        return AABB(enclosingMin, enclosingMax);
    };

    bool isOverlapping(const AABB& other) {
        return (this->min.x <= other.max.x && this->max.x >= other.min.x) &&
         (this->min.y <= other.max.y && this->max.y >= other.min.y) &&
         (this->min.z <= other.max.z && this->max.z >= other.min.z);
    }
private:
    glm::vec3 min; 
    glm::vec3 max;
    // std::vector<Face> faces;

};

