#pragma once

#include "GLProgram.h"

class GLProgram_Passthrough: public GLProgram
{
public:
	GLProgram_Passthrough() : GLProgram(){}
	virtual ~GLProgram_Passthrough(){}

protected:
	virtual void bindUniformBlocks()
	{
		this->bindUniformBlock(GLUniformBlockHelper::TYPE_PROJECTION);
	}
	virtual void fillUniforms()
	{
		GLState* glState = Singleton<GLState>::Instance();
		glm::mat4 modelToWorldMatrix = glState->modelToWorldMatrix;
		glm::mat4 worldToCameraMatrix = glState->worldToCameraMatrix;
		glm::mat4 modelToCameraMatrix = worldToCameraMatrix * modelToWorldMatrix;

		//Model to camera
		GLuint modelToCameraMatrixUniform = glGetUniformLocation(this->program, "modelToCameraMatrix");
		glUniformMatrix4fv(modelToCameraMatrixUniform, 1, GL_FALSE, glm::value_ptr(modelToCameraMatrix));
	}
};