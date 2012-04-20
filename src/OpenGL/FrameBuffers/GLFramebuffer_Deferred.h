#pragma once

#include "GLFrameBuffer.h"

class GL_Framebuffer_Deferred : public GLFramebuffer
{
public:
	enum GBUFFER_TEXTURE_TYPE {
        GBUFFER_TEXTURE_TYPE_POSITION,
        GBUFFER_TEXTURE_TYPE_DIFFUSE,
        GBUFFER_TEXTURE_TYPE_NORMAL,
        GBUFFER_NUM_TEXTURES
    };

	GLFramebuffer_Deferred() : GLFramebuffer(){}
	virtual ~GLFramebuffer_Deferred(){}

	virtual void initialize()
	{
		GLFramebuffer::initialize();

		glGenFramebuffers(1, &fbo);    
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);


	}

private:
	GLuint m_textures[GBUFFER_NUM_TEXTURES];
    GLuint m_depthTexture;
};