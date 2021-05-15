#pragma once

#include "Intersection.h"
#include "Ray.h"

class Intersectable
{
public:
    virtual Intersection intersect(const Ray& ray) = 0;
};