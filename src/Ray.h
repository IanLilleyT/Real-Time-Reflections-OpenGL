#pragma once
#include "glm/glm.hpp"

class Ray
{
public:

	//Variables
	glm::vec3 origin;
	glm::vec3 direction;
	glm::vec3 inv_direction;
	glm::ivec3 sign;
	float directionDistance;

	//Methods
	Ray()
	{
		//Do nothing
	}
	Ray(glm::vec3 o, glm::vec3 d)
	{
		SetValues(o,d);
	}
	void SetValues(glm::vec3 o, glm::vec3 d)
	{
		origin = o;
		direction = d;
		inv_direction = glm::vec3(1/d.x, 1/d.y, 1/d.z);
		sign.x = (inv_direction.x < 0);
		sign.y = (inv_direction.y < 0);
		sign.z = (inv_direction.z < 0);
		directionDistance = glm::length(direction);
	}
	glm::vec3 GetPointAlongRay(float t)
	{
		return (this->origin + t*this->direction);
	}
	void Transform(glm::mat4 transformation)
	{
		glm::vec3 originTransformed = glm::vec3(transformation*glm::vec4(this->origin,1.0f));
		glm::vec3 directionTransformed = glm::normalize(glm::vec3(transformation*glm::vec4(this->direction,0.0)));
		this->SetValues(originTransformed,directionTransformed);
	}
	static Ray Transform(Ray ray, glm::mat4 transformation)
	{
		Ray r = Ray(ray);
		glm::vec3 originTransformed = glm::vec3(transformation*glm::vec4(r.origin,1.0f));
		glm::vec3 directionTransformed = glm::normalize(glm::vec3(transformation*glm::vec4(r.direction,0.0)));
		r.SetValues(originTransformed,directionTransformed);
		return r;
	}
};
