#pragma once

#include <glm/glm.hpp>

#include "../RenderObject.h"

class Cylinder : public RenderObject
{
public:
	Cylinder();
	Cylinder(std::string name, std::string material, std::string program);
	virtual ~Cylinder();

protected:
	static std::string className;
};