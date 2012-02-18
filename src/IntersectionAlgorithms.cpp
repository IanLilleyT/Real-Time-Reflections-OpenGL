#include "IntersectionAlgorithms.h"

IntersectionData IntersectionAlgorithms::RaySphereIntersect(Ray& ray, glm::mat4 T)
{
	IntersectionData intersectionData;
	intersectionData.t = -1;

	glm::mat4 invT = glm::inverse(T);
	Ray invRay = Ray::Transform(ray,invT);

	float radius = 0.5f;
	float a = glm::dot(invRay.direction,invRay.direction);
	float b = 2*glm::dot(invRay.direction,invRay.origin);
	float c = glm::dot(invRay.origin,invRay.origin)-radius*radius;

	//Quadratic formula
	float insideSqrt = b*b-4*a*c;
	if(insideSqrt < 0)
		return intersectionData;

	float t1 = (-b + std::sqrt(insideSqrt))/(2*a);
	float t2 = (-b - std::sqrt(insideSqrt))/(2*a);
	float tMin = IntersectionAlgorithms::getMinT((t1 > 0), (t2 > 0), t1, t2);
	if(tMin <= 0) 
		return intersectionData;

	glm::vec3 intersectionPointTransformed = invRay.GetPointAlongRay(tMin);
	glm::vec3 intersectionPoint = glm::vec3(T*glm::vec4(intersectionPointTransformed,1.0f));
	glm::vec3 intersectionNormal = glm::normalize(glm::vec3(T*glm::vec4(intersectionPointTransformed,0.0f)));
	float finalPointDistance = glm::length(intersectionPoint - ray.origin);

	//Set intersection data
	intersectionData.t = finalPointDistance;
	intersectionData.point = intersectionPoint;
	intersectionData.normal = intersectionNormal;
	intersectionData.reflection = IntersectionAlgorithms::getReflection(ray.direction,intersectionNormal);
	return intersectionData;
}
bool IntersectionAlgorithms::PointInSphere(glm::vec3 point, glm::mat4 T)
{
	glm::mat4 invT = glm::inverse(T);
	glm::vec3 invPoint = glm::vec3(invT*glm::vec4(point,1.0f));
	float dist = glm::length(invPoint);
	return dist <= 0.5f;
}

//Helpers
float IntersectionAlgorithms::getMinT(bool expression1, bool expression2, float t1, float t2)
{
	float tMin = -1;
	if(!expression1 && !expression2) return -1;
	else if(!expression1 && expression2) tMin = t2;
	else if(expression1 && !expression2) tMin = t1;
	else tMin = (std::min)(t1,t2);
	return tMin;
}
bool IntersectionAlgorithms::equal(float val1, float val2)
{
	float epsilon = .001f;
	return abs(val1-val2) < epsilon;
}
glm::vec3 IntersectionAlgorithms::getReflection(glm::vec3 initialDir, glm::vec3 normal)
{
	float epsilon = .001f;
	glm::vec3 reflectedDir = initialDir - 2.0f*normal*glm::dot(initialDir,normal);
	reflectedDir = glm::normalize(reflectedDir);
	return reflectedDir;
}