#pragma once

#include "GLFramebuffer.h"

class GLFramebuffer_DepthBuffer : public GLFramebuffer
{
public:

	GLFramebuffer_DepthBuffer() : GLFramebuffer(){}
	virtual ~GLFramebuffer_DepthBuffer(){}

	virtual void initialize()
	{
		GLFramebuffer::initialize();

		//Screen dimensions
		GLView* glView = Singleton<GLView>::Instance();
		glm::ivec2 windowDimensions = glView->getWindowDimensions();

		//Generate and bind fbo
		glGenFramebuffers(1, &fbo);    
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

		//Generate textures
		glGenTextures(1, &this->depthTexture);

		//Depth attachment
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, windowDimensions.x, windowDimensions.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
		
		//Set draw buffers
		this->bindForWriting();

		//Error checking
		this->checkFBOErrors();

		//Unbind FBO
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}
	void bindForWriting()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	}
	void bindForReading(int textureUnit)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		
		//Depth
		glActiveTexture(GL_TEXTURE0 + textureUnit);		
		glBindTexture(GL_TEXTURE_2D, this->depthTexture);
	}
private:
	GLuint depthTexture;
};