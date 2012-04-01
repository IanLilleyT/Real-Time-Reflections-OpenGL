#pragma once

#include "GLFramebuffer.h"

class GLFramebuffer_Reflection : public GLFramebuffer
{
public:
	GLFramebuffer_Reflection() : GLFramebuffer(){}
	virtual ~GLFramebuffer_Reflection(){}

	virtual void initialize()
	{
		GLFramebuffer::initialize();
		
		//Create FBO
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
		
		//Window dimensions
		glm::ivec2 windowDimensions = Singleton<GLCamera>::Instance()->getWindowDimensions();
		GLuint width = (GLuint)windowDimensions.x;
		GLuint height = (GLuint)windowDimensions.y;

		//Color
		glGenTextures(1, &colorTexture);
		glBindTexture(GL_TEXTURE_2D, colorTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
			GL_RGBA, GL_UNSIGNED_INT_8_8_8_8 , NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//Depth
		glGenTextures(1, &depthTexture);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0,
			GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//Bind textures
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_TEXTURE_2D, depthTexture, 0);

		//Draw to correct color buffer
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		
		//Error checking
		GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
			printf("FB error, status: 0x%x\n", status);
		
		//Unbind buffer
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); 
	}
	virtual void bindForReading()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		//Bind color
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorTexture);

		//Bind depth
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
	}
	virtual void bindForWriting()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	}

private:
	GLuint colorTexture;
	GLuint depthTexture;
};