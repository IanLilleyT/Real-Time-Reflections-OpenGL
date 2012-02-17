#pragma once

#include <math.h>
#include <algorithm>

#include <glm/glm.hpp>

#include "Ray.h"

struct IntersectionData
{
public:
	//All in world coordinates
	float t; 
	glm::vec3 point;
	glm::vec3 normal;
	Ray reflectedRay;
};

class IntersectionAlgorithms
{
public:
	static IntersectionData RaySphereIntersect(Ray& ray, glm::mat4 T);
	static bool PointInSphere(glm::vec3 point, glm::mat4 T);
	
private:
	static float getMinT(bool expression1, bool expression2, float t1, float t2);
	static bool equal(float val1, float val2);
	static Ray getReflectedRay(glm::vec3 initialRayDir, glm::vec3 normal, glm::vec3 intersectionPoint);
};