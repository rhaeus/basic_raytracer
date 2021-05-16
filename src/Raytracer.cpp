#include "Raytracer.h"

#include <glm/glm.hpp>
#include <limits>
#include <memory>

#include"Air.h"

Raytracer::Raytracer(int width_, int height_, std::shared_ptr<Scene> scene_, int maxRecursions_, int superSamples_)
    : width(width_), height(height_), scene(scene_), maxRecursions(maxRecursions_), superSamples(superSamples_)
{
    // superSamples  must be square number, casts that many rays per pixel
    bvh = std::make_shared<BVH>(BVH(scene_->getObjects()));
}

inline
float deg2rad(const float &deg)
{ return deg * M_PI / 180.0f; }

void Raytracer::trace(glm::vec3* buffer) 
{
    glm::vec3* pixel = buffer;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            *(pixel++) = castRayAtPixel(x, y);
        }
    }
}

glm::vec3 Raytracer::castRayAtPixel(int x, int y) 
{
    float scale = tan(deg2rad(scene->getCamera().getFov() * 0.5f));
    float imageAspectRatio = width / (float)height;

    glm::vec3 rayOrigin(0,0,0);

    float x_coord_left = (2 * (x) / (float)width - 1) * imageAspectRatio * scale;
    float x_coord_right = (2 * (x+1.0) / (float)width - 1) * imageAspectRatio * scale;

    float y_coord_top = (1 - 2 * (y) / (float)height) * scale;
    float y_coord_bottom = (1 - 2 * (y+1.0) / (float)height) * scale;

    float pixelWidth = x_coord_right - x_coord_left;
    float pixelHeight = y_coord_top - y_coord_bottom;

    float weight = 1.0f / superSamples;

    float samplesPerDimension = sqrt(superSamples);
    float stepX = pixelWidth/samplesPerDimension;
    float stepY = pixelHeight/samplesPerDimension;

    glm::vec3 color(0,0,0);

    for (int c = 0; c < (int)samplesPerDimension; ++c) {
        for (int r = 0; r < (int)samplesPerDimension; ++r) {
            float rndX = rand() / double(RAND_MAX);
            float rndY = rand() / double(RAND_MAX);

            float sampleX = x_coord_left + (r+rndX) * stepX;
            float sampleY = y_coord_top - (c+rndY) * stepY;

            glm::vec3 rayDir(sampleX, sampleY, 1);
            rayDir -= rayOrigin;
            rayDir = glm::normalize(rayDir);

            scene->getCamera().getCameraRay(sampleX, sampleY, rayOrigin, rayDir);

            color += weight * castRay(Ray(rayOrigin, rayDir, maxRecursions, std::make_shared<Material>(Air())));
        }
    }

    return color;
}

glm::vec3 Raytracer::castRay(const Ray& ray) 
{
    rayCount++;

    Intersection closestInter = getClosestIntersection(ray);
    if (closestInter.intersectionOccurred) {
        return calculateColor(closestInter);
    } else {
        return glm::vec3(0, 0, 0);
    }

}

Intersection Raytracer::getClosestIntersection(const Ray& ray)  
{
    return bvh->intersect(ray);

    float minDist = std::numeric_limits<float>::max();

    // Intersection closest(ray);

    // for (auto object : scene->getObjects()) {
    //     auto intersection = object->intersect(ray);
    //     if (intersection.intersectionOccurred) {
    //         if (intersection.getDistance() < minDist ){
    //             minDist = intersection.getDistance();
    //             closest = intersection;
    //         }  
    //     }
    // }

	// return closest;
}

glm::vec3 Raytracer::calculateColor(const Intersection& intersection)  
{
    glm::vec3 objectColor = intersection.getObject()->getColor(intersection.getPosition());
    const glm::vec3& ambientColor = calculateAmbientLighting(intersection, objectColor);
    const glm::vec3& diffuseAndSpecularColor = calculateDiffuseAndSpecularLighting(intersection, objectColor);
    const glm::vec3& reflectedColor = calculateReflectiveRefractiveLighting(intersection);

    return ambientColor + diffuseAndSpecularColor + reflectedColor;
}

glm::vec3 Raytracer::calculateAmbientLighting(const Intersection& intersection, const glm::vec3& objectColor)  
{
   return objectColor * 0.2f;
}


// in and normal are expected to be normalized
// in is the direction out of the surface..I know...
glm::vec3 Raytracer::reflect(const glm::vec3& in, const glm::vec3& normal) 
{
    return 2.0f * glm::dot(in,normal) * normal - in;
}

glm::vec3 Raytracer::refract(const glm::vec3& in, const glm::vec3& normal, float n1, float n2) 
{
   float n = n1 / n2;
   float cosI = -glm::dot(normal,in);
   float sinT2 = n * n * (1.0 - cosI * cosI);

   if (sinT2 > 1.0) {
      std::cerr << "Bad refraction vector!" << std::endl;
      exit(EXIT_FAILURE);
   }

   double cosT = sqrt(1.0 - sinT2);
   return in * n + normal * (float)(n * cosI - cosT);
}

void Raytracer::fresnel(const glm::vec3& in, const glm::vec3& normal, float n1, float n2, float& r, float& t) 
{
   float n = n1 / n2;
   float cosI = -glm::dot(normal,in);
   float sinT2 = n * n * (1.0 - cosI * cosI);

   if (sinT2 > 1.0) {
      // Total Internal Reflection.
      r = 1.0f;
      t = 0.0f;
      return;
   }

   float cosT = sqrt(1.0 - sinT2);
   float r0rth = (n1 * cosI - n2 * cosT) / (n1 * cosI + n2 * cosT);
   float rPar = (n2 * cosI - n1 * cosT) / (n2 * cosI + n1 * cosT);

   r = (r0rth * r0rth + rPar * rPar) / 2.0;
   t = 1.0f - r;
}

bool Raytracer::isInShadow(const glm::vec3& point, const glm::vec3& lightPosition) 
{
    glm::vec3 lightOffset = lightPosition - point;
    float lightDistance = glm::length(lightOffset);
    glm::vec3 L = glm::normalize(lightOffset);

    // check if in shadow
    Ray shadowRay(point, L, 1, nullptr);
    Intersection shadowInter = getClosestIntersection(shadowRay);
    if (shadowInter.intersectionOccurred) {
        float distToClosestInter = glm::distance(point, shadowInter.getPosition());
        if (lightDistance > distToClosestInter) {
            return true; // in shadow
        }

    }
    return false; //not in shadow
}

glm::vec3 Raytracer::calculateDiffuseAndSpecularLighting(const Intersection& intersection, const glm::vec3& objectColor) 
{
    // Phong Illumination

    glm::vec3 diffuseColor(0.0, 0.0, 0.0);
    glm::vec3 specularColor(0.0, 0.0, 0.0);

    for (auto light : scene->getLights())
    {
        // get normal and light direction
        const glm::vec3& N = intersection.getNormal();
        const glm::vec3 lightOffset = light->getPosition() - intersection.getPosition();
        const glm::vec3 L = glm::normalize(lightOffset);

        auto material = intersection.getObject()->getMaterial();

        float dot_nl = glm::dot(N, L);

        // Intersection is facing light
        if (dot_nl >= 0.0f) 
        {
            // sample all light points
            int total = light->getSamplePoints().size();
            int hit = 0;
            for (auto p : light->getSamplePoints()) {
                // check if in shadow
                if (!isInShadow(intersection.getPosition(), p)){
                    hit++;
                }
            }

            float intensity = (float)hit / (float)total;

            if (intensity == 0) {
                continue; // completely in shadow;
            }


            // diffuse color
            diffuseColor += objectColor * dot_nl * light->getRadiosity() * intensity;

            // specular color
            float p = material->getShininess(); 

            if (p < 0) {
                // not shiny
                continue;
            }
            
            // float cosNL = std::max(dot_nl, float(0));
            const glm::vec3 V = -intersection.getRay().getDirection();
            const glm::vec3 r = glm::normalize(reflect(L, N));

            float cosrv = std::max(float(glm::dot(r, V)), float(0));
            float cosrv_p = pow(cosrv, p);

            specularColor += cosrv_p *  light->getRadiosity() * intensity;

        } 


    }
    return specularColor + diffuseColor;
}

glm::vec3 Raytracer::calculateReflectiveRefractiveLighting(const Intersection& intersection) 
{
   float objectReflectiveness = intersection.getObject()->getMaterial()->getReflectiveness();

   float startIor= intersection.getRay().getTravelMaterial()->getIndexOfRefraction();
   float objectIor = intersection.getObject()->getMaterial()->getIndexOfRefraction();

   int reflectionsRemaining = intersection.getRay().getRemainingReflections();

   if (objectReflectiveness < 0 && objectIor < 0 || reflectionsRemaining <= 0) {
      return glm::vec3(0,0,0);
   }

   // Default to exclusively reflective values.
   float r = objectReflectiveness; // reflective portion
   float t = 0;// transmissive/refractive portion

   // calculate reflective and transmissive component using fresnel
   if (objectIor >= 0) {
        fresnel(intersection.getRay().getDirection(), intersection.getNormal(), startIor, objectIor, r, t);
   }

    // no reflection or refraction happening
   if (t <= 0 && r <= 0) {
      return glm::vec3(0,0,0);
   }

   glm::vec3 reflectiveColor(0,0,0);
   glm::vec3 refractiveColor(0,0,0);

   if (r > 0) {
      const glm::vec3 reflected = reflect(-intersection.getRay().getDirection(), intersection.getNormal());
      const Ray reflectedRay(intersection.getPosition(), glm::normalize(reflected), reflectionsRemaining - 1, intersection.getRay().getTravelMaterial());
      reflectiveColor = castRay(reflectedRay) * r;
   }

   if (t > 0) {
        const glm::vec3 refracted = refract(intersection.getRay().getDirection(), intersection.getNormal(), startIor, objectIor);
        const Ray refractedRay(intersection.getPosition(), glm::normalize(refracted), 1, intersection.getObject()->getMaterial());
        refractiveColor = castRay(refractedRay) * t;
   }

   return reflectiveColor + refractiveColor;
}