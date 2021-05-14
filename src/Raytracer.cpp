#include "Raytracer.h"

#include <glm/glm.hpp>
#include <limits>
#include <memory>

#include"Air.h"

Raytracer::Raytracer(int width_, int height_, Camera camera_, std::shared_ptr<Scene> scene_, int maxRecursions_)
    : width(width_), height(height_), camera(camera_), scene(scene_), maxRecursions(maxRecursions_)
{
    superSamples = 1; // must be square number, casts that many rays per pixel
}

inline
float deg2rad(const float &deg)
{ return deg * M_PI / 180.0f; }

void Raytracer::trace(glm::vec3* buffer) 
{
    minX = 10000000000;
    maxX = -10000000000;
    minY = 10000000000;
    maxY = -10000000000;
    rayCount = 0;
    // https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays
    // const glm::mat4& cameraToWorld = camera.getViewMatrix();
    glm::vec3* pixel = buffer;

    // float scale = tan(deg2rad(camera.getFov() * 0.5));
    // float imageAspectRatio = width / (float)height;

    // float scale = tan(deg2rad(60 * 0.5)); //H/2f
    // float imageAspectRatio = width / (float)height; 

    // transform the ray origin (which is also the camera origin)
    // by transforming the point with coordinates (0,0,0) to world-space using the
    // camera-to-world matrix.
    // glm::vec4 origin = cameraToWorld * glm::vec4(0,0,0,1);
    // glm::vec3 rayOrigin = glm::vec3(origin.x/origin.w, origin.y/origin.w, origin.z/origin.w);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            *(pixel++) = castRayAtPixel(x, y);
        }
    }
    std::cout << "Ray count: " << rayCount << std::endl;
    std::cout << "minx " << minX << std::endl;
    std::cout << "maxx " << maxX << std::endl;
    std::cout << "miny " << minY << std::endl;
    std::cout << "maxy " << maxY << std::endl;
}

glm::vec3 Raytracer::castRayAtPixel(int x, int y) 
{
    // float imageAspectRatio = width / (float)height; // assuming width > height 
    // float Px = (2 * ((x + 0.5) / width) - 1) * tan(camera.getFov() / 2 * M_PI / 180) * imageAspectRatio; 
    // float Py = (1 - 2 * ((y + 0.5) / height)) * tan(camera.getFov() / 2 * M_PI / 180); 


    // // Px = x - width/2.0f;
    // // Py = y - height/2.0f;

    // glm::vec3 rayOrigin(0,0,0); 
    // glm::vec3 rayDirection = glm::vec3(Px, Py, 1);// - rayOrigin; // note that this just equal to Vec3f(Px, Py, -1); 
    // rayDirection = glm::normalize(rayDirection); // it's a direction so don't forget to normalize 

    // camera.getCameraRay(x,y, rayOrigin, rayDirection);

    // return castRay(Ray(rayOrigin, rayDirection, maxRecursions, std::make_shared<Material>(Air())));

    float scale = tan(deg2rad(camera.getFov() * 0.5f));
    // scale=1.0f;
    float imageAspectRatio = width / (float)height;
    // imageAspectRatio = 1.0f;
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
            float sampleX = x_coord_left + (r+0.5) * stepX;
            float sampleY = y_coord_top - (c+0.5) * stepY;

            if (sampleX < minX)
                minX = sampleX;
            if (sampleX > maxX)
                maxX = sampleX;
            if (sampleY < minY)
                minY = sampleY;
            if (sampleY > maxY)
                maxY = sampleY;
            
            glm::vec3 rayDir(sampleX, sampleY, 1);
            rayDir -= rayOrigin;
            rayDir = glm::normalize(rayDir);

            camera.getCameraRay(sampleX, sampleY, rayOrigin, rayDir);

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
        return glm::vec3(0,0,0);
    }

}

Intersection Raytracer::getClosestIntersection(const Ray& ray)  
{
    float minDist = std::numeric_limits<float>::max();
    bool result = false;

    Intersection closest(ray);

    for (auto object : scene->getObjects()) {
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

bool Raytracer::isInShadow(const Ray& ray, double lightDistance) 
{
    Intersection closestInter = getClosestIntersection(ray);
    return closestInter.intersectionOccurred && closestInter.getDistance() < lightDistance;
}

glm::vec3 Raytracer::getDiffuseAndSpecularLighting(Intersection intersection, const glm::vec3& color) 
{
   glm::vec3 diffuseColor(0.0, 0.0, 0.0);
   glm::vec3 specularColor(0.0, 0.0, 0.0);

   for (auto light : scene->getLights()) {

      glm::vec3 lightOffset = light->getPosition() - intersection.position;
      double lightDistance = lightOffset.length();
      /**
       * TODO: Be careful about normalizing lightOffset too.
       */
      glm::vec3 lightDirection = glm::normalize(lightOffset);
      double dotProduct = glm::dot(intersection.getNormal(), lightDirection);

      /**
       * Intersection is facing light.
       */
      if (dotProduct >= 0.0f) {
         Ray shadowRay = Ray(intersection.position, lightDirection, 1,
          intersection.getRay().getTravelMaterial());

         if (isInShadow(shadowRay, lightDistance)) {
            /**
             * Position is in shadow of another object - continue with other lights.
             */
            continue;
         }

         diffuseColor = (diffuseColor + (color * (float)dotProduct)) * light->getFlux();
         specularColor = specularColor + getSpecularLighting(intersection, light);
      }
   }

    // return diffuseColor;
    // return specularColor;
   return diffuseColor + specularColor;
}

glm::vec3 Raytracer::getSpecularLighting(Intersection intersection, std::shared_ptr<Light> light) 
{
   glm::vec3 specularColor(0.0, 0.0, 0.0);
   double shininess = intersection.getObject() ->getMaterial()->getShininess();

   if (shininess == -1) {
      /* Don't perform specular lighting on non shiny objects. */
      return specularColor;
   }

   glm::vec3 view = glm::normalize(intersection.getRay().getOrigin() - intersection.position);
   glm::vec3 lightOffset = light->getPosition() - intersection.position;
   glm::vec3 reflected = reflect(glm::normalize(lightOffset), intersection.getNormal());

   double dot = glm::dot(view, reflected);

   if (dot <= 0) {
      return specularColor;
   }

   double specularAmount = pow(dot, shininess) * light->getFlux();

   specularColor.r = specularAmount;
   specularColor.g = specularAmount;
   specularColor.b = specularAmount;

   return specularColor;
}

glm::vec3 Raytracer::calculateColor(const Intersection& intersection)  
{
    glm::vec3 objectColor = intersection.getObject()->getColor(intersection.getPosition());
    glm::vec3 ambientColor = calculateAmbientLighting(intersection, objectColor);
    glm::vec3 diffuseAndSpecularColor = calculateDiffuseAndSpecularLighting(intersection, objectColor);
    // glm::vec3 diffuseAndSpecularColor =  getDiffuseAndSpecularLighting(intersection, objectColor);
    glm::vec3 reflectedColor = calculateReflectiveRefractiveLighting(intersection);

    // if (reflectedColor.r > 0.00001 && reflectedColor.g > 0.00001 && reflectedColor.b > 0.00001) {
    //     std::cout << reflectedColor.r << ", " << reflectedColor.g << ", " << reflectedColor.b << std::endl;
    // }

    // return diffuseAndSpecularColor;
    // return ambientColor;
    // return ambientColor + diffuseAndSpecularColor;
    // return ambientColor + reflectedColor;

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
    // auto inN = glm::normalize(in);
    // auto N = glm::normalize(normal);
    // return in - 2.0f * glm::dot(in, normal);
//    return 2.0f * glm::dot(inN,N) * N - inN; 
//    return -2.0f * glm::dot(inN,N) * N + inN; 
    return 2.0f * glm::dot(in,normal) * normal - in;
}

glm::vec3 Raytracer::refract(glm::vec3 in, glm::vec3 normal, float n1, float n2) 
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

void Raytracer::fresnel(glm::vec3 in, glm::vec3 normal, float n1, float n2, float& r, float& t) 
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

glm::vec3 Raytracer::calculateDiffuseAndSpecularLighting(const Intersection& intersection, const glm::vec3& objectColor) 
{
    // Phong Illumination

    glm::vec3 diffuseColor(0.0, 0.0, 0.0);
    glm::vec3 specularColor(0.0, 0.0, 0.0);

    for (auto light : scene->getLights())
    {
        // get normal and light direction
        const glm::vec3& N = intersection.getNormal();
        glm::vec3 lightOffset = light->getPosition() - intersection.getPosition();
        float lightDistance = glm::length(lightOffset);
        glm::vec3 L = glm::normalize(lightOffset);

        auto material = intersection.getObject()->getMaterial();

        float dot_nl = glm::dot(N, L);

        // Intersection is facing light
        if (dot_nl >= 0.0f) 
        {
            // check if in shadow
            Ray shadowRay(intersection.getPosition(), L, 1, intersection.getRay().getTravelMaterial());
            Intersection shadowInter = getClosestIntersection(shadowRay);
            if (shadowInter.intersectionOccurred) {
                float distToClosestInter = glm::distance(intersection.getPosition(), shadowInter.getPosition());
                if (lightDistance > distToClosestInter) {
                    // std::cout << "shadow" << std::endl;
                    // we are in the shadow, continue with next light
                    continue;
                }

            }
            // not in shadow

            // diffuse color
            diffuseColor += objectColor * dot_nl * light->getRadiosity();

            // specular color
            float p = material->getShininess(); 

            if (p < 0) {
                // not shiny
                continue;
            }
            
            // float cosNL = std::max(dot_nl, float(0));
            glm::vec3 V = -intersection.getRay().getDirection();
            glm::vec3 r = glm::normalize(reflect(L, N));

            float cosrv = std::max(float(glm::dot(r, V)), float(0));
            float cosrv_p = pow(cosrv, p);

            specularColor += cosrv_p *  light->getRadiosity();

        } 
        // else {
        //     std::cout << "whut" << std::endl;
        // }

    }
    // return specularColor;
    // if (specularColor != glm::vec3(0,0,0)){
    // std::cout << specularColor.r << ", " << specularColor.g << ", " << specularColor.b <<std::endl;
    // }
    return specularColor + diffuseColor;//glm::vec3(0.5,0.5,0.5);
}

glm::vec3 Raytracer::calculateReflectiveRefractiveLighting(const Intersection& intersection) 
{
   float objectReflectiveness = intersection.getObject()->getMaterial()->getReflectiveness();

   float startIor= intersection.getRay().getTravelMaterial()->getIndexOfRefraction();
   float objectIor = intersection.getObject()->getMaterial()->getIndexOfRefraction();

   int reflectionsRemaining = intersection.getRay().getRemainingReflections();

//    if (reflectionsRemaining < 5) {
    //    std::cout << "hmm " << reflectionsRemaining<< std::endl;
//    }

   /**
    * Don't perform lighting if the object is not reflective or refractive or we have
    * hit our recursion limit.
    */
//    if (reflectionsRemaining <= 0) {
//        return glm::vec3(1,0,0);
//    }
   if (objectReflectiveness < 0 && objectIor < 0 || reflectionsRemaining <= 0) {
      return glm::vec3(0,0,0);
   }

   // Default to exclusively reflective values.
   float r = objectReflectiveness; // reflective portion
   float t = 0; // transmissive/refractive portion

   // Refractive index overrides the reflective property.
   if (objectIor >= 0) {
        fresnel(intersection.getRay().getDirection(), intersection.getNormal(), startIor, objectIor, r, t);
   }

   // No ref{ra,le}ctive properties - bail early.
   if (t <= 0 && r <= 0) {
      return glm::vec3(0,0,0);
   }

   glm::vec3 reflectiveColor(0,0,0);
   glm::vec3 refractiveColor(0,0,0);

   if (r > 0) {
      glm::vec3 reflected = reflect(-intersection.getRay().getDirection(), intersection.getNormal());
      Ray reflectedRay(intersection.getPosition(), glm::normalize(reflected), reflectionsRemaining - 1, intersection.getRay().getTravelMaterial());
      reflectiveColor = castRay(reflectedRay) * r;
   }

   if (t > 0) {
    //    std::cout << "t" << std::endl;
        glm::vec3 refracted = refract(intersection.getRay().getDirection(), intersection.getNormal(), startIor, objectIor);
        Ray refractedRay(intersection.getPosition(), glm::normalize(refracted), 1, intersection.getObject()->getMaterial());
        refractiveColor = castRay(refractedRay) * t;
   }

   return reflectiveColor + refractiveColor;
}