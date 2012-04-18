#pragma once

#include <vector>
#include <iostream>

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
	void checkFBOErrors()
	{
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status == GL_FRAMEBUFFER_UNDEFINED)
			std::cout << "GL_FRAMEBUFFER_UNDEFINED" << std::endl;
		else if(status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" << std::endl;
		else if(status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" << std::endl;
		else if(status == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER)
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" << std::endl;
		else if(status == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER)
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" << std::endl;
		else if(status == GL_FRAMEBUFFER_UNSUPPORTED)
			std::cout << "GL_FRAMEBUFFER_UNSUPPORTED" << std::endl;
		else if(status == GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE)
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE" << std::endl;
		else if(status == GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE)
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE" << std::endl;
		else if(status == GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS)
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS" << std::endl;
	}
	void generateColorTexture(GLuint* texture)
	{
		glm::ivec2 windowDimensions = Singleton<GLCamera>::Instance()->getWindowDimensions();
		GLuint width = (GLuint)windowDimensions.x;
		GLuint height = (GLuint)windowDimensions.y;

		glGenTextures(1, texture);
		glBindTexture(GL_TEXTURE_2D, *texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
			GL_RGBA, GL_UNSIGNED_INT_8_8_8_8 , NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	void generateDepthTexture(GLuint* texture)
	{
		glm::ivec2 windowDimensions = Singleton<GLCamera>::Instance()->getWindowDimensions();
		GLuint width = (GLuint)windowDimensions.x;
		GLuint height = (GLuint)windowDimensions.y;

		glGenTextures(1, texture);
		glBindTexture(GL_TEXTURE_2D, *texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0,
			GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	GLuint fbo;
};