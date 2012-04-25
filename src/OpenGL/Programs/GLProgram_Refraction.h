#pragma once

#include "GLProgram.h"

class GLProgram_Refraction: public GLProgram
{
public:
	GLProgram_Refraction() : GLProgram(){}
	virtual ~GLProgram_Refraction(){}

protected:
	virtual void bindUniformBlocks()
	{
		this->bindUniformBlock(GLUniformBlockHelper::TYPE_PROJECTION);
	}
	virtual void fillUniforms()
	{
		GLState* glState = Singleton<GLState>::Instance();

		//position texture
		int positionTexture = glState->positionTextureUnit;
		GLuint positionTextureUniform = glGetUniformLocation(this->program, "positionTexture");
		glUniform1i(positionTextureUniform, positionTexture);

		//normal texture
		int normalTexture = glState->normalTextureUnit;
		GLuint normalTextureUniform = glGetUniformLocation(this->program, "normalTexture");
		glUniform1i(normalTextureUniform, normalTexture);

		//other texture
		int otherTexture = glState->otherTextureUnit;
		GLuint otherTextureUniform = glGetUniformLocation(this->program, "otherTexture");
		glUniform1i(otherTextureUniform, otherTexture);

		//depth texture
		int depthTexture = glState->depthTextureUnit;
		GLuint depthTextureUniform = glGetUniformLocation(this->program, "depthTexture");
		glUniform1i(depthTextureUniform, depthTexture);

		//color buffer texture
		int colorBufferTexture = glState->colorBufferTextureUnit;
		GLuint colorBufferTextureUniform = glGetUniformLocation(this->program, "colorBufferTexture");
		glUniform1i(colorBufferTextureUniform, colorBufferTexture);
	}
};