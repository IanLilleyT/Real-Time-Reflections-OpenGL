#pragma once

#include <glm/glm.hpp>

#include "../RenderObject.h"

class Cube : public RenderObject
{
public:
	Cube();
	Cube(std::string name, std::string material, std::string program);
	virtual ~Cube();

protected:
	static std::string className;
};