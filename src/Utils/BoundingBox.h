#pragma once

#include <cfloat>

#include <glm/glm.hpp> //For math

class BoundingBox
{
public:
	BoundingBox(glm::vec3 min, glm::vec3 max)
	{
		this->min = min;
		this->max = max;
	}
	BoundingBox(){}
	~BoundingBox(){}

	glm::vec3 min;
	glm::vec3 max;
};