#pragma once

#include "glm/glm.hpp" //For math

class Ray
{
public:
	glm::vec3 origin;
	glm::vec3 direction;
	glm::vec3 inv_direction;
	glm::ivec3 sign;

	Ray(){}
	Ray(glm::vec3 o, glm::vec3 d)
	{
		origin = o;
		direction = d;
		inv_direction = glm::vec3(1/d.x, 1/d.y, 1/d.z);
		sign.x = (inv_direction.x < 0);
		sign.y = (inv_direction.y < 0);
		sign.z = (inv_direction.z < 0);
	}
	glm::vec3 getPointAlongRay(float t)
	{
		//Gets the point distance t away from the origin in the ray's direction
		return (this->origin + t*this->direction);
	}
	Ray transform(glm::mat4 transformation)
	{
		glm::vec3 originTransformed = glm::vec3(transformation*glm::vec4(this->origin,1.0f));
		glm::vec3 directionTransformed = glm::normalize(glm::vec3(transformation*glm::vec4(this->direction,0.0)));
		Ray newRay = Ray(originTransformed,directionTransformed);
		return newRay;
	}
};

