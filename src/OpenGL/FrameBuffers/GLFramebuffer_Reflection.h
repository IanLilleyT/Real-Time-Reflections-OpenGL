#pragma once

#include <utility>

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
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		
		//Generate textures
		this->generateColorTexture(&colorTexture0);
		this->generateDepthTexture(&depthTexture0);
		this->generateColorTexture(&colorTexture1);
		this->generateDepthTexture(&depthTexture1);

		//Draw to correct color buffer
		this->bindWriteTextures(GLFramebuffer_Reflection::TEXTURE_GROUP0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		//Error checking
		this->checkFBOErrors();
                    
		//Unbind buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0); 
	}

	void bindForReadingAndWriting(GLenum colorTextureUnit, GLenum depthTextureUnit, int textureGroupRead, int textureGroupWrite)
	{
		this->bindReadTextures(colorTextureUnit, depthTextureUnit, textureGroupRead);
		this->bindForWriting(textureGroupWrite);
	}
	void bindForWriting(int textureGroup)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		this->bindWriteTextures(textureGroup);
	}
	void bindForReading(GLenum colorTextureUnit, GLenum depthTextureUnit, int textureGroup)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		this->bindReadTextures(colorTextureUnit, depthTextureUnit, textureGroup);
	}
	
	static const int TEXTURE_GROUP0 = 0;
	static const int TEXTURE_GROUP1 = 1;

private:
	void bindReadTextures(GLenum colorTextureUnit, GLenum depthTextureUnit, int textureGroup)
	{
		std::pair<GLuint,GLuint> textures = this->getTexturesByGroup(textureGroup);
		glActiveTexture(colorTextureUnit); //color
		glBindTexture(GL_TEXTURE_2D, textures.first);
		glActiveTexture(depthTextureUnit); //depth
		glBindTexture(GL_TEXTURE_2D, textures.second);
	}
	void bindWriteTextures(int textureGroup)
	{
		std::pair<GLuint,GLuint> textures = this->getTexturesByGroup(textureGroup);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures.first, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_TEXTURE_2D, textures.second, 0);
	}
	std::pair<GLuint,GLuint> getTexturesByGroup(int textureGroup)
	{
		GLuint colorTexture = textureGroup == GLFramebuffer_Reflection::TEXTURE_GROUP0 ? colorTexture0 : colorTexture1;
		GLuint depthTexture = textureGroup == GLFramebuffer_Reflection::TEXTURE_GROUP0 ? depthTexture0 : depthTexture1;
		return std::pair<GLuint,GLuint>(colorTexture, depthTexture);
	}

	//Two different texture groups for ping-ponging
	GLuint colorTexture0;
	GLuint depthTexture0;
	GLuint colorTexture1;
	GLuint depthTexture1;
};