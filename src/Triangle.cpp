#include "Triangle.h"

Triangle::Triangle(glm::vec3 v0_, glm::vec3 v1_, glm::vec3 v2_, glm::vec2 t0_, glm::vec2 t1_, glm::vec2 t2_, std::shared_ptr<Material> material_) 
    : Renderable(material_)
{
	vertexCoords.push_back(v0_);
	vertexCoords.push_back(v1_);
	vertexCoords.push_back(v2_);

	textureCoords.push_back(t0_);
	textureCoords.push_back(t1_);
	textureCoords.push_back(t2_);

    glm::vec3 e1 = v1_-v0_;
    glm::vec3 e2 = v2_-v0_;
	normal = glm::normalize( glm::cross( e2, e1 ) );

	// compute bounds
	glm::vec3 min = v0_;
	glm::vec3 max = v0_;
	for (auto v : vertexCoords) {
		if (v.x < min.x) 
			min.x = v.x;
		if (v.x > max.x) 
			max.x = v.x;

		if (v.y < min.y) 
			min.y = v.y;
		if (v.y > max.y) 
			max.y = v.y;
		
		if (v.z < min.z) 
			min.z = v.z;
		if (v.z > max.z) 
			max.z = v.z;
	}
	bounds = std::make_shared<AABB>(min, max);
}

Triangle::Triangle(glm::vec3 v0_, glm::vec3 v1_, glm::vec3 v2_, std::shared_ptr<Material> material_)
	: Triangle(v0_, v1_, v2_, glm::vec2(0,0), glm::vec2(0,0), glm::vec2(0,0), material_) 
{}

Intersection Triangle::intersect(const Ray& ray) {
	rayTriangleIntersectionTest++;

	const glm::vec3 e1 = vertexCoords[1] - vertexCoords[0];
	const glm::vec3 e2 = vertexCoords[2] - vertexCoords[0];
	const glm::vec3 b = ray.getOrigin() - vertexCoords[0];
	glm::mat3 A(-ray.getDirection(), e1, e2);
	glm::vec3 x = glm::inverse(A) * b; // x = (t u v)^T

	float t = x.x;
	float u = x.y;
	float v = x.z;

	bool intersect = u >= 0 && v >= 0 && (u + v) <= 1 && t > 0;

	if (intersect) {
		rayTriangleIntersections++;
		
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

float getTriangleArea(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	return glm::length(glm::cross(ab, ac))/2.0f;
}

glm::vec3 Triangle::getColor(glm::vec3 pos)  
{
	// calculate texture coordinate

	//barycentric interpolation
	float totalArea = getTriangleArea(vertexCoords[0], vertexCoords[1], vertexCoords[2]);
	float alpha = getTriangleArea(vertexCoords[1], vertexCoords[2], pos) / totalArea;
	float beta = getTriangleArea(vertexCoords[0], pos, vertexCoords[2]) / totalArea;
	float gamma = getTriangleArea(vertexCoords[0], vertexCoords[1], pos) / totalArea;

	float x = alpha + beta + gamma;

	// nearest neighbor
	glm::vec2 tex = alpha * textureCoords[0] + beta * textureCoords[1] + gamma * textureCoords[2];

	if (tex.x > 1.0f){
		// happens sometimes that this triggers but when printed it's 1, I guess imprecisions maybe
		// std::cout << "whups x " << tex.x <<std::endl;
		tex.x = 1.0f;
	}

	if (tex.y > 1.0f) {
		// std::cout << "whups y " << tex.y <<std::endl;
		tex.y = 1.0f;
	}
	
	
    return material->getColor(pos, tex); 
}

glm::vec3 Triangle::getNormal(glm::vec3 pos)  
{
    return normal; //TODO normal map
}
