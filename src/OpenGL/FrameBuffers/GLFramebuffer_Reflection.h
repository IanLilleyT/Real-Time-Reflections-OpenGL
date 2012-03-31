#pragma once

#include "GLFramebuffer.h"

class GLFramebuffer_Color : public GLFramebuffer
{
public:
	GLFramebuffer_Color() : GLFramebuffer(){}
	virtual ~GLFramebuffer_Color(){}

	virtual void initialize()
	{
		GLFramebuffer::initialize();
		
		//Create FBO
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

		//Window dimensions
		glm::ivec2 windowDimensions = Singleton<GLCamera>::Instance()->getWindowDimensions();
		GLuint windowWidth = (GLuint)windowDimensions.x;
		GLuint windowHeight = (GLuint)windowDimensions.y;

		//Color
		glGenTextures(1, &colorTexture);
		glBindTexture(GL_TEXTURE_2D, colorTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowHeight, windowWidth, 0,
			GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//Depth
		glGenTextures(1, &depthTexture);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, windowHeight, windowWidth, 0,
			GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//Bind textures
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_TEXTURE_2D, depthTexture, 0);

		//Disable drawing to regular color buffer
		glDrawBuffer(GL_NONE);
		
		//Error checking
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
			printf("FB error, status: 0x%x\n", status);
	}
	virtual void bindForReading()
	{
		//Bind color
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, colorTexture);

		//Bind depth
		glActiveTexture(1);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
	}

private:
	GLuint colorTexture;
	GLuint depthTexture;
};