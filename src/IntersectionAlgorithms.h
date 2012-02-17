#pragma once

#include <math.h>
#include <algorithm>

#include <glm/glm.hpp>

#include "Ray.h"

struct IntersectionData
{
public:
	float t; 
	glm::vec3 point; //World coordinates
	glm::vec3 normal; //World coordinates
};

class IntersectionAlgorithms
{
public:
	static IntersectionData RaySphereIntersect(Ray& ray, glm::mat4 T);
	static bool PointInSphere(glm::vec3 point, glm::mat4 T);
	
private:
	static float getMinT(bool expression1, bool expression2, float t1, float t2);
	static bool equal(float val1, float val2);
};