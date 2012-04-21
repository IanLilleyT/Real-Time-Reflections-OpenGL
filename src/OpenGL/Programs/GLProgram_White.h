#pragma once

#include "GLProgram.h"

class GLProgram_White: public GLProgram
{
public:
	GLProgram_White() : GLProgram(){}
	virtual ~GLProgram_White(){}

protected:
	virtual void fillUniforms()
	{
		//Model to clip
		GLState* glState = Singleton<GLState>::Instance();
		GLCamera* glCamera = Singleton<GLCamera>::Instance();
		glm::mat4 modelToWorldMatrix = glState->modelToWorldMatrix;
		glm::mat4 worldToCameraMatrix = glCamera->getWorldToCameraMatrix();
		glm::mat4 cameraToClipMatrix = glCamera->getCameraToClipMatrix();
		glm::mat4 modelToClipMatrix = cameraToClipMatrix * worldToCameraMatrix * modelToWorldMatrix;

		GLuint modelToClipMatrixUniform = glGetUniformLocation(this->program, "modelToClipMatrix");
		glUniformMatrix4fv(modelToClipMatrixUniform, 1, GL_FALSE, glm::value_ptr(modelToClipMatrix));
	}
};