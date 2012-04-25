#pragma once

#include "GLProgram.h"

class GLProgram_ShadowMap : public GLProgram
{
public:
	GLProgram_ShadowMap() : GLProgram(){}
	virtual ~GLProgram_ShadowMap(){}

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

		//shadow map texture
		int shadowMapTexture = glState->shadowMapTextureUnit;
		GLuint shadowMapTextureUniform = glGetUniformLocation(this->program, "shadowMapTexture");
		glUniform1i(shadowMapTextureUniform,shadowMapTexture);

		//color buffer texture
		int colorBufferTexture = glState->colorBufferTextureUnit;
		GLuint colorBufferTextureUniform = glGetUniformLocation(this->program, "colorBufferTexture");
		glUniform1i(colorBufferTextureUniform, colorBufferTexture);

		//depth texture
		int depthTexture = glState->depthTextureUnit;
		GLuint depthTextureUniform = glGetUniformLocation(this->program, "depthTexture");
		glUniform1i(depthTextureUniform, depthTexture);

		//camera to world matrix
		glm::mat4 cameraToWorldMatrix = glm::inverse(glState->worldToCameraMatrix);
		GLuint cameraToWorldMatrixUniform = glGetUniformLocation(this->program, "cameraToWorldMatrix");
		glUniformMatrix4fv(cameraToWorldMatrixUniform, 1, GL_FALSE, glm::value_ptr(cameraToWorldMatrix));

		//light world to camera matrix
		glm::mat4 lightWorldToCameraMatrix = glState->lightWorldToCameraMatrix;
		GLuint lightWorldToCameraMatrixUniform = glGetUniformLocation(this->program, "lightWorldToCameraMatrix");
		glUniformMatrix4fv(lightWorldToCameraMatrixUniform, 1, GL_FALSE, glm::value_ptr(lightWorldToCameraMatrix));
	}
};