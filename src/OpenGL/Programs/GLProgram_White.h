#pragma once

#include "GLProgram.h"

class GLProgram_White: public GLProgram
{
public:
	GLProgram_White() : GLProgram(){}
	virtual ~GLProgram_White(){}
	virtual void createVAO()
	{
		GLProgram::createVAO();
		vao->addAttribute(this->getAttribute("position", 3, GL_FLOAT));
	}

protected:
	virtual void fillUniforms()
	{
		//Model to clip
		GLState* glState = Singleton<GLState>::Instance();
		glm::mat4 modelToWorldMatrix = glState->getModelToWorldMatrix();
		glm::mat4 worldToCameraMatrix = glState->getWorldToCameraMatrix();
		glm::mat4 cameraToClipMatrix = glState->getCameraToClipMatrix();
		glm::mat4 modelToClipMatrix = cameraToClipMatrix * worldToCameraMatrix * modelToWorldMatrix;

		GLuint modelToClipMatrixUniform = glGetUniformLocation(this->program, "modelToClipMatrix");
		glUniformMatrix4fv(modelToClipMatrixUniform, 1, GL_FALSE, glm::value_ptr(modelToClipMatrix));
	}
};