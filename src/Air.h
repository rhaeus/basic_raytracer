#pragma once
#include "Material.h"

class Air : public Material
{
public:
    Air() { 
        ior = 1.0f;
    }

};