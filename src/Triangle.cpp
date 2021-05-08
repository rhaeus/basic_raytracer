#include "Triangle.h"

Triangle::Triangle(glm::vec3 v0_, glm::vec3 v1_, glm::vec3 v2_, glm::vec2 vt0_, glm::vec2 vt1_, glm::vec2 vt2_, std::shared_ptr<Material> material_) 
    : Renderable(material_), v0(v0_), v1(v1_), v2(v2_), vt0(vt0_), vt1(vt1_), vt2(vt2_)
{
    glm::vec3 e1 = v1_-v0_;
    glm::vec3 e2 = v2_-v0_;
	normal = glm::normalize( glm::cross( e2, e1 ) );
}

Triangle::Triangle(glm::vec3 v0_, glm::vec3 v1_, glm::vec3 v2_, std::shared_ptr<Material> material_)
	: Triangle(v0_, v1_, v2_, glm::vec2(0,0), glm::vec2(0,0), glm::vec2(0,0), material_) 
{}

Intersection Triangle::intersect(const Ray& ray) {
	const glm::vec3 e1 = v1 - v0;
	const glm::vec3 e2 = v2 - v0;
	const glm::vec3 b = ray.getOrigin() - v0;
	glm::mat3 A(-ray.getDirection(), e1, e2);
	glm::vec3 x = glm::inverse(A) * b; // x = (t u v)^T

	float t = x.x;
	float u = x.y;
	float v = x.z;

	bool intersect = u >= 0 && v >= 0 && (u + v) <= 1 && t > 0;

	if (intersect) {
		glm::vec3 n = normal;
		// Normal needs to be flipped if this is a refractive ray.
		if (glm::dot(ray.getDirection(), normal) > 0) {
			n = n * -1.0f;
		}
		glm::vec3 point = ray.getOrigin() + (ray.getDirection() * t);
		return Intersection (ray, t, point, n, shared_from_this(), true);
	}

	return Intersection(ray);
}

glm::vec3 Triangle::getColor(glm::vec3 pos)  
{
    return material->getColor(glm::vec2(0,0)); //TODO color map
}

glm::vec3 Triangle::getNormal(glm::vec3 pos)  
{
    return normal; //TODO normal map
}
