#pragma once

#include "GLProgram.h"

class GLProgram_MotionBlur : public GLProgram
{
public:
	GLProgram_MotionBlur() : GLProgram(){}
	virtual ~GLProgram_MotionBlur(){}

protected:
	virtual void bindUniformBlocks()
	{
		this->bindUniformBlock(GLUniformBlockHelper::TYPE_PROJECTION);
	}
	virtual void fillUniforms()
	{
		GLState* glState = Singleton<GLState>::Instance();
		

		//color buffer texture
		int colorBufferTexture = glState->colorBufferTextureUnit;
		GLuint colorBufferTextureUniform = glGetUniformLocation(this->program, "colorBufferTexture");
		glUniform1i(colorBufferTextureUniform, colorBufferTexture);

		//depth texture
		int depthTexture = glState->depthTextureUnit;
		GLuint depthTextureUniform = glGetUniformLocation(this->program, "depthTexture");
		glUniform1i(depthTextureUniform, depthTexture);

		//world to clip matrix
		glm::mat4 worldToClipMatrix = glState->cameraToClipMatrix*glState->worldToCameraMatrix;
		GLuint worldToClipMatrixUniform = glGetUniformLocation(this->program, "worldToClipMatrix");
		glUniformMatrix4fv(worldToClipMatrixUniform, 1, GL_FALSE, glm::value_ptr(worldToClipMatrix));

		//old world to clip matrix
		glm::mat4 oldWorldToClipMatrix = glState->cameraToClipMatrix*glState->oldWorldToCameraMatrix;
		GLuint oldWorldToClipMatrixUniform = glGetUniformLocation(this->program, "oldWorldToClipMatrix");
		glUniformMatrix4fv(oldWorldToClipMatrixUniform, 1, GL_FALSE, glm::value_ptr(oldWorldToClipMatrix));

		//clip to world matrix
		glm::mat4 clipToWorldMatrix = glm::inverse(worldToClipMatrix);
		GLuint clipToWorldMatrixUniform = glGetUniformLocation(this->program, "clipToWorldMatrix");
		glUniformMatrix4fv(clipToWorldMatrixUniform, 1, GL_FALSE, glm::value_ptr(clipToWorldMatrix));
	}
};