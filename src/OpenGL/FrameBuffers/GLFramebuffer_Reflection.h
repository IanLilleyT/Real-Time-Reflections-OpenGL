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
		this->checkFBOErrors();
	}
	std::pair<GLuint,GLuint> getTexturesByGroup(int textureGroup)
	{
		GLuint colorTexture = textureGroup == GLFramebuffer_Reflection::TEXTURE_GROUP0 ? colorTexture0 : colorTexture1;
		GLuint depthTexture = textureGroup == GLFramebuffer_Reflection::TEXTURE_GROUP0 ? depthTexture0 : depthTexture1;
		return std::pair<GLuint,GLuint>(colorTexture, depthTexture);
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

	//Two different texture groups for ping-ponging
	GLuint colorTexture0;
	GLuint depthTexture0;
	GLuint colorTexture1;
	GLuint depthTexture1;
};