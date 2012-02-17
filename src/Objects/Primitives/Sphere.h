#pragma once

#include <glm/glm.hpp>

#include "../RenderObject.h"

class Sphere : public RenderObject
{
public:
	Sphere();
	Sphere(std::string name, std::string material, std::string program);
	virtual ~Sphere();

protected:
	static std::string className;
};