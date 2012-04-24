#pragma once

#include "GLFramebuffer.h"

class GLFramebuffer_ColorBuffer : public GLFramebuffer
{
public:

	GLFramebuffer_ColorBuffer() : GLFramebuffer(){}
	virtual ~GLFramebuffer_ColorBuffer(){}

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
		glGenTextures(1, &colorBufferTexture);

		//Color attachments
		glBindTexture(GL_TEXTURE_2D, colorBufferTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowDimensions.x, windowDimensions.y, 0, GL_RGBA, GL_FLOAT, NULL);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowDimensions.x, windowDimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBufferTexture, 0);

		//Set draw buffers
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		
		//Error checking
		this->checkFBOErrors();

		//Unbind FBO
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}
	void bindForWriting()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	}
	void bindForReading(GLenum textureUnit)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		
		//Colors
		glActiveTexture(GL_TEXTURE0 + textureUnit);		
		glBindTexture(GL_TEXTURE_2D, colorBufferTexture);
	}
private:
	GLuint colorBufferTexture;
};