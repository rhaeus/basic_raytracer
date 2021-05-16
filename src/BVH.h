#pragma once

#include "Renderable.h"
#include "AABB.h"
#include "Intersection.h"


#include <vector>
#include <memory>

extern unsigned long long bailEarly;

class Node 
{
public:
    Node(std::shared_ptr<AABB> bounds) : isLeaf(true), depth(0), aabb(bounds)
    {
        children = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
    };

    std::shared_ptr<AABB> aabb;
    bool isLeaf;
    int depth;

    static const int NUMBER_OF_CHILDREN = 8;
    
    std::array<std::shared_ptr<Node>,NUMBER_OF_CHILDREN> children;
    // std::vector<std::shared_ptr<Node>> children;
    std::vector<std::shared_ptr<Renderable>> content;

    Intersection intersect(const Ray& ray) 
    {
        if (isLeaf) {
            // loop through all content and return closest intersection (if any)
            return intersectContent(ray);
        } else {
            // intersect children aabb
            return intersectChildren(ray);
        }
    }

    Intersection intersectChildren(const Ray& ray)  
    {
        float minDist = std::numeric_limits<float>::max();

        Intersection closest(ray);

        for (auto child : children) {
            if (child) {
                if (child->aabb->intersects(ray)) {
                    auto intersection = child->intersect(ray);
                    if (intersection.intersectionOccurred) {
                        if (intersection.getDistance() < minDist ){
                            minDist = intersection.getDistance();
                            closest = intersection;
                        } 
                    }
                } else {
                    // std::cout << "bail from child intersect" << std::endl;
                    bailEarly++;
                }
            }
        }

        return closest;
    }

    Intersection intersectContent(const Ray& ray)  
    {
        float minDist = std::numeric_limits<float>::max();

        Intersection closest(ray);

        for (auto object : content) {
            auto intersection = object->intersect(ray);
            if (intersection.intersectionOccurred) {
                if (intersection.getDistance() < minDist ){
                    minDist = intersection.getDistance();
                    closest = intersection;
                }
                    
            }
        }

        return closest;
    }
};

class BVH
{
public:
    BVH(std::vector<std::shared_ptr<Renderable>> objects);
    Intersection intersect(const Ray& ray);
private:
    const int MAX_DEPTH = 16;
    std::shared_ptr<Node> root;
    void insertObjects(std::vector<std::shared_ptr<Renderable>> objects);
    void insert(std::shared_ptr<Node> node, std::shared_ptr<Renderable> object, int depth);
    void computeBounds(std::shared_ptr<Node> node);
};