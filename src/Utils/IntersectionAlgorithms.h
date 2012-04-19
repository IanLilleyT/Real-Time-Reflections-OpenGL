#pragma once

#include <algorithm> //For min/max

#include <glm/glm.hpp> //For math

#include "Ray.h" //For intersection calculations
#include "BoundingBox.h"

struct IntersectionData
{
public:
	//All in world coordinates
	bool valid; 
	glm::vec3 point;
	glm::vec3 normal;
	glm::vec3 reflection;
	float distanceAlongRay;
};

class IntersectionAlgorithms
{
public:
	//Sees if a point is contained by the volume by shooting tha ray from opposite directions
	static IntersectionData PointInSphere(glm::vec3 point, glm::vec3 direction, glm::mat4 T);
	static IntersectionData PointInCylinder(glm::vec3 point, glm::vec3 direction, glm::mat4 T);
	
	static IntersectionData RaySphereIntersect(Ray& ray, glm::mat4 T);
	static IntersectionData RayCylinderIntersect(Ray& ray, glm::mat4 T);
	static IntersectionData RayBoxIntersect(Ray& ray, glm::mat4 T, BoundingBox* boundingBox);

	static bool IsValidIntersection(IntersectionData data);
	
private:
	static float IntersectionAlgorithms::getMinT(bool expression1, bool expression2, float t1, float t2);
	static bool IntersectionAlgorithms::equal(float val1, float val2);
	static glm::vec3 IntersectionAlgorithms::getReflection(glm::vec3 initialDir, glm::vec3 normal);

	static float EPSILON;
};