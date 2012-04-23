#pragma once

#include "GLFramebuffer.h"

class GLFramebuffer_GBuffer : public GLFramebuffer
{
public:
	
	enum GBUFFER_TEXTURE_TYPE {
        GBUFFER_TEXTURE_TYPE_POSITION,
        GBUFFER_TEXTURE_TYPE_NORMAL,
		GBUFFER_TEXTURE_TYPE_DIFFUSE_COLOR,
		GBUFFER_TEXTURE_TYPE_SPECULAR_COLOR,
		GBUFFER_TEXTURE_TYPE_OTHER,
        GBUFFER_NUM_TEXTURES
    };

	GLFramebuffer_GBuffer() : GLFramebuffer(){}
	virtual ~GLFramebuffer_GBuffer(){}

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
		glGenTextures(GBUFFER_NUM_TEXTURES, textures); //colors
		glGenTextures(1, &depthTexture); //depth

		//Color attachments
		GLenum drawBuffers[GBUFFER_NUM_TEXTURES];
		for (unsigned int i = 0 ; i < GBUFFER_NUM_TEXTURES ; i++) 
		{
			glBindTexture(GL_TEXTURE_2D, textures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowDimensions.x, windowDimensions.y, 0, GL_RGBA, GL_FLOAT, NULL);
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowDimensions.x, windowDimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i], 0);
			drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
		}

		//Depth attachment
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, windowDimensions.x, windowDimensions.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
	
		//Set draw buffers
		glDrawBuffers(GBUFFER_NUM_TEXTURES, drawBuffers);
		
		//Error checking
		this->checkFBOErrors();

		//Unbind FBO
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}
	void bindForWriting()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	}
	void bindForReading()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		
		//Colors
		for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
			glActiveTexture(GL_TEXTURE0 + i);		
			glBindTexture(GL_TEXTURE_2D, textures[i]);
		}

		//Depth
		GLenum first = GL_TEXTURE0;
		GLenum thing = GL_TEXTURE0 + GBUFFER_NUM_TEXTURES;
		glActiveTexture(GL_TEXTURE0 + GBUFFER_NUM_TEXTURES);		
		glBindTexture(GL_TEXTURE_2D, depthTexture);
	}
private:
	GLuint textures[GBUFFER_NUM_TEXTURES];
    GLuint depthTexture;
};