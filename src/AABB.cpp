#include "AABB.h"

AABB::AABB(glm::vec3 min_, glm::vec3 max_)
    : min(min_), max(max_)
{
    // float width = max_.x - min_.x;
    // float height = max_.y - min_.y;
    // float depth = max_.z - min_.z;

    // glm::vec3 v0 = min_;
    // glm::vec3 v1 = min_ + glm::vec3(width, 0, 0); 
    // glm::vec3 v2 = min_ + glm::vec3(width, height, 0);
    // glm::vec3 v3 = min_ + glm::vec3(0, height, 0);
    // glm::vec3 v4 = min_ + glm::vec3(0, 0, depth);
    // glm::vec3 v5 = min_ + glm::vec3(width, 0, depth);
    // glm::vec3 v6 = min_ + glm::vec3(width, height, depth);
    // glm::vec3 v7 = min_ + glm::vec3(0, height, depth);

    // // front
    // faces.push_back(Face(v0, v1-v0, v3-v0));
    // // left
    // faces.push_back(Face(v4, v0-v4, v7-v4));
    // // right
    // faces.push_back(Face(v1, v5-v1, v2-v1));
    // // back
    // faces.push_back(Face(v5, v4-v5, v6-v5));
    // // top
    // faces.push_back(Face(v3, v2-v3, v7-v3));
    // // bottom
    // faces.push_back(Face(v4, v5-v4, v0-v4));
    
}

bool AABB::intersects(const Ray& ray) const 
{
    rayAABBIntersectionTest++;
    // return true;
    // for (auto f : faces) {
    //     if (f.intersects(ray)) {
    //         return true;
    //     }
    // }
    // return false;

    // r.dir is unit direction vector of ray
    const glm::vec3& dirfrac = ray.getDirFrac();
    const glm::vec3& rayOrig = ray.getOrigin();
    float t = 0;

    // lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
    // r.org is origin of ray
    float t1 = (min.x - rayOrig.x)*dirfrac.x;
    float t2 = (max.x - rayOrig.x)*dirfrac.x;
    float t3 = (min.y - rayOrig.y)*dirfrac.y;
    float t4 = (max.y - rayOrig.y)*dirfrac.y;
    float t5 = (min.z - rayOrig.z)*dirfrac.z;
    float t6 = (max.z - rayOrig.z)*dirfrac.z;

    float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
    float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

    // if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
    if (tmax < 0)
    {
        t = tmax;
        return false;
    }

    // if tmin > tmax, ray doesn't intersect AABB
    if (tmin > tmax)
    {
        t = tmax;
        return false;
    }

    t = tmin;
    return true;
}

glm::vec3 AABB::getCenter() 
{
    // return min + 0.5f * (max - min);
    return (min + max) / 2.0f;
}
