#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <gl3w/gl3w.h>

#include "../../Utils/Singleton.h"
#include "../GLCamera.h"

class GLFramebuffer
{
public:
	GLFramebuffer(){}
	virtual ~GLFramebuffer(){}

	virtual void initialize(){}

protected:
	GLuint fbo;
};