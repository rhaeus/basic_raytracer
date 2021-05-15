#include "Raytracer.h"

#include <glm/glm.hpp>
#include <limits>
#include <memory>

#include"Air.h"

Raytracer::Raytracer(int width_, int height_, std::shared_ptr<Scene> scene_, int maxRecursions_)
    : width(width_), height(height_), scene(scene_), maxRecursions(maxRecursions_)
{
    superSamples = 1; // must be square number, casts that many rays per pixel
}

inline
float deg2rad(const float &deg)
{ return deg * M_PI / 180.0f; }

void Raytracer::trace(glm::vec3* buffer) 
{
    rayCount = 0;
    rayTriangleIntersectionTest = 0;
    rayTriangleIntersections = 0;
    raySphereIntersectionTest = 0;
    raySphereIntersections = 0;

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
    std::cout << "Ray-Triangle Intersection Tests: " << rayTriangleIntersectionTest << std::endl;
    std::cout << "Ray-Triangle Intersections: " << rayTriangleIntersections << std::endl;
    std::cout << "Ray-Sphere Intersection Tests: " << raySphereIntersectionTest << std::endl;
    std::cout << "Ray-Sphere Intersections: " << raySphereIntersections << std::endl;
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

    float scale = tan(deg2rad(scene->getCamera().getFov() * 0.5f));
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
        return glm::vec3(1, 0, 195/255.0f);
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

glm::vec3 Raytracer::calculateColor(const Intersection& intersection)  
{
    glm::vec3 objectColor = intersection.getObject()->getColor(intersection.getPosition());
    glm::vec3 ambientColor = calculateAmbientLighting(intersection, objectColor);
    glm::vec3 diffuseAndSpecularColor = calculateDiffuseAndSpecularLighting(intersection, objectColor);
    glm::vec3 reflectedColor = calculateReflectiveRefractiveLighting(intersection);

    return ambientColor + diffuseAndSpecularColor + reflectedColor;
}

glm::vec3 Raytracer::calculateAmbientLighting(const Intersection& intersection, const glm::vec3& objectColor)  
{
   return objectColor * 0.2f;
}

// out = incidentVec - 2.f * Dot(incidentVec, normal) * normal;

// in and normal are expected to be normalized
// in is the direction out of the surface..I know...
glm::vec3 Raytracer::reflect(const glm::vec3& in, const glm::vec3& normal) 
{
    // auto inN = glm::normalize(in);
    // auto N = glm::normalize(normal);
    // return in - 2.0f * glm::dot(in, normal) * normal;
//    return 2.0f * glm::dot(inN,N) * N - inN; 
//    return -2.0f * glm::dot(inN,N) * N + inN; 
    return 2.0f * glm::dot(in,normal) * normal - in;
}

// inline void Refract(
//   VEC3 &out, const VEC3 &incidentVec, const VEC3 &normal, float eta)
// {
//   float N_dot_I = Dot(normal, incidentVec);
//   float k = 1.f - eta * eta * (1.f - N_dot_I * N_dot_I);
//   if (k < 0.f)
//     out = VEC3(0.f, 0.f, 0.f);
//   else
//     out = eta * incidentVec - (eta * N_dot_I + sqrtf(k)) * normal;
// }

glm::vec3 Raytracer::refract(glm::vec3 in, glm::vec3 normal, float n1, float n2) 
{
    // glm::vec3 incidentVec = in;
    // // normal = -normal;
    // float eta = n1 / n2;

    // glm::vec3 result(0,0,0);

    // float N_dot_I = glm::dot(normal, incidentVec);
    // float k = 1.f - eta * eta * (1.f - N_dot_I * N_dot_I);
    // if (k < 0.f)
    //     return result;
    // else
    // {
    //     result = eta * incidentVec - (eta * N_dot_I + sqrtf(k)) * normal;
    //     return result;
    // }
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
    // // in = -in;
    // // normal = -normal;
    // float cosi = glm::clamp(glm::dot(in, normal),-1.0f, 1.0f); 
    // float etai = n1, etat = n2; 
    // if (cosi > 0) { std::swap(etai, etat); } 
    // // Compute sini using Snell's law
    // float sint = etai / etat * sqrtf(std::max(0.f, 1.0f - cosi * cosi)); 
    // // Total internal reflection
    // if (sint >= 1) { 
    //     r = 1.0f; 
    //     t = 1.0f-r;
    // } 
    // else { 
    //     float cost = sqrtf(std::max(0.f, 1 - sint * sint)); 
    //     cosi = fabsf(cosi); 
    //     float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost)); 
    //     float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost)); 
    //     r = (Rs * Rs + Rp * Rp) / 2.0f; 
    //     t = 1.0f-r;
    // } 

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
        glm::vec3 lightOffset = light->getPosition() - intersection.getPosition();
        float lightDistance = glm::length(lightOffset);
        glm::vec3 L = glm::normalize(lightOffset);

        auto material = intersection.getObject()->getMaterial();

        float dot_nl = glm::dot(N, L);

        // Intersection is facing light
        if (dot_nl >= 0.0f) 
        {
            // check if in shadow
            if (isInShadow(intersection.getPosition(), light->getPosition()))
            {
                // we are in the shadow, continue with next light
                continue;
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


    }
    return specularColor + diffuseColor;
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
   float t = 0;//1.0f-r;; // transmissive/refractive portion

   // Refractive index overrides the reflective property.
   if (objectIor >= 0) {
        fresnel(intersection.getRay().getDirection(), intersection.getNormal(), startIor, objectIor, r, t);
   }
//    r=0.01;
//    t=1.0f-r;

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