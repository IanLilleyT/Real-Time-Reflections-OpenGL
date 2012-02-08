#pragma once

#include <glm/glm.hpp>

#include "RenderObject.h"

class Jello: public RenderObject
{
public:
	Jello();
	virtual ~Jello();

	virtual void update();
	virtual void render();

	void initialize(std::string shape, glm::vec3 origin, glm::vec3 size, glm::uvec3 divisions); 
	
	std::string getShape();
	glm::vec3 getOrigin();
	glm::vec3 getSize();
	glm::uvec3 getDivisions();

	static std::string SHAPE_CUBE;
	static std::string SHAPE_SPHERE;

private:
	std::string shape;
	glm::vec3 origin;
	glm::vec3 size;
	glm::uvec3 divisions;
};