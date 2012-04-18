#pragma once

#include <utility>

#include "GLFramebuffer.h"

class GLFramebuffer_ShadowMap : public GLFramebuffer
{
public:
	GLFramebuffer_ShadowMap() : GLFramebuffer(){}
	virtual ~GLFramebuffer_ShadowMap(){}

	virtual void initialize()
	{
		GLFramebuffer::initialize();
		
		//Create FBO
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		
		//Generate textures
		this->generateDepthTexture(&depthTexture);

		//Draw to correct color buffer
		this->bindWriteTexture();

		//Error checking
		this->checkFBOErrors();
                    
		//Unbind buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0); 
	}

	void bindForReading(GLenum depthTextureUnit)
	{
		//glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		this->bindReadTexture(depthTextureUnit);
	}
	void bindForWriting()
	{
		//glDrawBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		this->bindWriteTexture();
	}

private:	

	void bindReadTexture(GLenum depthTextureUnit)
	{
		glActiveTexture(depthTextureUnit); //depth
		glBindTexture(GL_TEXTURE_2D, depthTexture);
	}
	void bindWriteTexture()
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_TEXTURE_2D, depthTexture, 0);
	}

	GLuint depthTexture;
};