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

	void setSize(glm::vec3 dimensions);

private:

};