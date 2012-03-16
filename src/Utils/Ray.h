#pragma once

#include "glm/glm.hpp" //For math

class Ray
{
public:

	//Variables
	glm::vec3 origin;
	glm::vec3 direction;
	glm::vec3 inv_direction;
	glm::ivec3 sign;

	//Methods
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
	glm::vec3 GetPointAlongRay(float t)
	{
		return (this->origin + t*this->direction);
	}
	static Ray Transform(Ray& ray, glm::mat4 transformation)
	{
		glm::vec3 originTransformed = glm::vec3(transformation*glm::vec4(ray.origin,1.0f));
		glm::vec3 directionTransformed = glm::normalize(glm::vec3(transformation*glm::vec4(ray.direction,0.0)));
		Ray newRay = Ray(originTransformed,directionTransformed);
		return newRay;
	}
};

