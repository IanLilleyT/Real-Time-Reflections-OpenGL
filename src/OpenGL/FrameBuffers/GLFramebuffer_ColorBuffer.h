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
		glGenTextures(1, &colorBufferTexture1);
		glGenTextures(1, &colorBufferTexture2);

		//Color attachments
		glBindTexture(GL_TEXTURE_2D, colorBufferTexture1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowDimensions.x, windowDimensions.y, 0, GL_RGBA, GL_FLOAT, NULL);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowDimensions.x, windowDimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, colorBufferTexture2);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowDimensions.x, windowDimensions.y, 0, GL_RGBA, GL_FLOAT, NULL);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowDimensions.x, windowDimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		//Set draw buffers
		this->bindForWriting();
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		
		//Error checking
		this->checkFBOErrors();

		//Unbind FBO
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}
	void bindForWriting()
	{
		this->swapTextures();
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->currentTexture, 0);
	}
	void bindForReading(int textureUnit)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		
		//Colors
		glActiveTexture(GL_TEXTURE0 + textureUnit);		
		glBindTexture(GL_TEXTURE_2D, this->currentTexture);
	}
	void bindForReadingAndWriting(int textureUnit)
	{
		this->bindForReading(textureUnit);
		this->bindForWriting();
	}
	void swapTextures()
	{
		if(this->currentTexture == colorBufferTexture1)
			this->currentTexture = colorBufferTexture2;
		else
			this->currentTexture = colorBufferTexture1;
	}
private:
	GLuint colorBufferTexture1;
	GLuint colorBufferTexture2;
	GLuint currentTexture;
};