#include "IntersectionAlgorithms.h"

float IntersectionAlgorithms::EPSILON = .001f;

IntersectionData IntersectionAlgorithms::PointInSphere(glm::vec3 point, glm::vec3 direction, glm::mat4 T)
{
	direction = glm::normalize(direction);
	Ray ray1 = Ray(point - direction*1000.0f, direction);
	Ray ray2 = Ray(point + direction*1000.0f, -direction);
	IntersectionData intersection1 = IntersectionAlgorithms::RaySphereIntersect(ray1,T);
	IntersectionData intersection2 = IntersectionAlgorithms::RaySphereIntersect(ray2,T);
	if(intersection1.valid && intersection2.valid)
	{
		float originalDistance1 = glm::length(ray1.origin - point);
		float originalDistance2 = glm::length(ray2.origin - point);
		float finalDistance1 = glm::length(ray1.origin - intersection1.point);
		float finalDistance2 = glm::length(ray2.origin - intersection2.point);
		intersection1.valid = (originalDistance1 >= finalDistance1) && (originalDistance2 >= finalDistance2);
	}
	else
	{
		intersection1.valid = false;
		intersection2.valid = false;
	}
	return intersection1;
}
IntersectionData IntersectionAlgorithms::PointInCylinder(glm::vec3 point, glm::vec3 direction, glm::mat4 T)
{
	direction = glm::normalize(direction);
	Ray ray1 = Ray(point - direction*1000.0f, direction);
	Ray ray2 = Ray(point + direction*1000.0f, -direction);
	IntersectionData intersection1 = IntersectionAlgorithms::RayCylinderIntersect(ray1,T);
	IntersectionData intersection2 = IntersectionAlgorithms::RayCylinderIntersect(ray2,T);
	if(intersection1.valid && intersection2.valid)
	{
		float originalDistance1 = glm::length(ray1.origin - point);
		float originalDistance2 = glm::length(ray2.origin - point);
		float finalDistance1 = glm::length(ray1.origin - intersection1.point);
		float finalDistance2 = glm::length(ray2.origin - intersection2.point);
		intersection1.valid = (originalDistance1 >= finalDistance1) && (originalDistance2 >= finalDistance2);
	}
	else
	{
		intersection1.valid = false;
		intersection2.valid = false;
	}
	return intersection1;
}
IntersectionData IntersectionAlgorithms::RaySphereIntersect(Ray& ray, glm::mat4 T)
{
	IntersectionData intersectionData;
	intersectionData.valid = false;

	glm::mat4 invT = glm::inverse(T);
	Ray invRay = Ray::Transform(ray,invT);

	float radius = 0.5f;
	float a = glm::dot(invRay.direction,invRay.direction);
	float b = 2*glm::dot(invRay.direction,invRay.origin);
	float c = glm::dot(invRay.origin,invRay.origin)-radius*radius;

	//Quadratic formula
	float insideSqrt = b*b-4*a*c;
	if(insideSqrt < 0) return intersectionData; //END

	float t1 = (-b + std::sqrt(insideSqrt))/(2*a);
	float t2 = (-b - std::sqrt(insideSqrt))/(2*a);
	float tMin = IntersectionAlgorithms::getMinT((t1 > 0), (t2 > 0), t1, t2);
	if(tMin <= 0) return intersectionData; //END

	glm::vec3 intersectionPointTransformed = invRay.GetPointAlongRay(tMin);
	glm::vec3 intersectionPoint = glm::vec3(T*glm::vec4(intersectionPointTransformed,1.0f));
	glm::vec3 intersectionNormal = glm::normalize(glm::vec3(T*glm::vec4(intersectionPointTransformed,0.0f)));
	float finalPointDistance = glm::length(intersectionPoint - ray.origin);

	//Set intersection data
	intersectionData.valid = true;
	intersectionData.point = intersectionPoint;
	intersectionData.normal = intersectionNormal;
	intersectionData.reflection = IntersectionAlgorithms::getReflection(ray.direction,intersectionNormal);
	return intersectionData;
}
IntersectionData IntersectionAlgorithms::RayCylinderIntersect(Ray& ray, glm::mat4 T)
{
	IntersectionData intersectionData;
	intersectionData.valid = false;

	glm::mat4 invT = glm::inverse(T);
	Ray invRay = Ray::Transform(ray,invT);

	float cylinderRadius = 0.5f;
	float cylinderHeightHalf = 0.5f;

	float rayDirX = invRay.direction.x;
	float rayDirY = invRay.direction.y;
	float rayDirZ = invRay.direction.z;
	float rayOriginX = invRay.origin.x;
	float rayOriginY = invRay.origin.y;
	float rayOriginZ = invRay.origin.z;

	//Get the tMinSide
	float a = rayDirX*rayDirX + rayDirZ*rayDirZ;
	float b = 2*rayOriginX*rayDirX + 2*rayOriginZ*rayDirZ;
	float c = rayOriginX*rayOriginX + rayOriginZ*rayOriginZ - cylinderRadius*cylinderRadius;

	float tSideMin = -1;
	float insideSqrt = b*b-4*a*c;
	if(insideSqrt >= 0)
	{
		float t1 = (-b + std::sqrt(insideSqrt))/(2*a);
		float t2 = (-b - std::sqrt(insideSqrt))/(2*a);
		float y1 = invRay.GetPointAlongRay(t1).y;
		float y2 = invRay.GetPointAlongRay(t2).y;
		bool y1Valid = t1 > 0 && y1 >= -cylinderHeightHalf && y1 <= cylinderHeightHalf;
		bool y2Valid = t2 > 0 && y2 >= -cylinderHeightHalf && y2 <= cylinderHeightHalf;
		tSideMin = IntersectionAlgorithms::getMinT(y1Valid, y2Valid, t1, t2);
	}	

	//Get the tMin for the caps
	float tBottom;
	if(rayDirY == 0 && rayOriginY == -cylinderHeightHalf) tBottom = 0;
	else tBottom = (-cylinderHeightHalf - rayOriginY)/rayDirY;
	glm::vec3 pointBottom = invRay.GetPointAlongRay(tBottom);
	float xBottom = pointBottom.x;
	float zBottom = pointBottom.z;
	bool tBottomValid = (tBottom > 0) && (xBottom*xBottom + zBottom*zBottom < cylinderRadius*cylinderRadius);

	float tTop;
	if(rayDirY == 0 && rayOriginY == cylinderHeightHalf) tTop = 0;
	else tTop = (cylinderHeightHalf - rayOriginY)/rayDirY;
	glm::vec3 pointTop = invRay.GetPointAlongRay(tTop);
	float xTop = pointTop.x;
	float zTop = pointTop.z;
	bool tTopValid = (tTop > 0) && (xTop*xTop + zTop*zTop < cylinderRadius*cylinderRadius);

	float tCapMin = IntersectionAlgorithms::getMinT(tBottomValid, tTopValid, tBottom, tTop);

	//Get min t value
	float tMin = IntersectionAlgorithms::getMinT(tSideMin > 0, tCapMin > 0, tSideMin, tCapMin);
	if(tMin <= 0) return intersectionData;

	//Get the intersection val in world space
	glm::vec3 intersectionPointTransformed = invRay.GetPointAlongRay(tMin);
	glm::vec3 intersectionPoint = glm::vec3(T*glm::vec4(intersectionPointTransformed,1.0f));

	//Get normal
	glm::vec4 intersectionNormalTransformed;
	if(IntersectionAlgorithms::equal(tMin,tBottom))
		intersectionNormalTransformed = glm::vec4(0,-1,0,0);
	else if(IntersectionAlgorithms::equal(tMin,tTop))
		intersectionNormalTransformed = glm::vec4(0,1,0,0);
	else if(IntersectionAlgorithms::equal(tMin,tSideMin))
		intersectionNormalTransformed = glm::normalize(glm::vec4(intersectionPointTransformed.x, 0, intersectionPointTransformed.z, 0.0f));
	glm::vec3 intersectionNormal = glm::normalize(glm::vec3(T*intersectionNormalTransformed));
	float finalPointDistance = glm::length(intersectionPoint - ray.origin);
		
	//Set intersection data
	intersectionData.valid = true;
	intersectionData.point = intersectionPoint;
	intersectionData.normal = intersectionNormal;
	intersectionData.reflection = IntersectionAlgorithms::getReflection(ray.direction,intersectionNormal);
	return intersectionData;
}

//Helpers
float IntersectionAlgorithms::getMinT(bool expression1, bool expression2, float t1, float t2)
{
	float tMin = -1;
	if(!expression1 && !expression2) return -1;
	else if(!expression1 && expression2) tMin = t2;
	else if(expression1 && !expression2) tMin = t1;
	else tMin = std::min(t1,t2);
	return tMin;
}
bool IntersectionAlgorithms::equal(float val1, float val2)
{
	return abs(val1-val2) < IntersectionAlgorithms::EPSILON;
}
glm::vec3 IntersectionAlgorithms::getReflection(glm::vec3 initialDir, glm::vec3 normal)
{
	glm::vec3 reflectedDir = initialDir - 2.0f*normal*glm::dot(initialDir,normal);
	reflectedDir = glm::normalize(reflectedDir);
	return reflectedDir;
}