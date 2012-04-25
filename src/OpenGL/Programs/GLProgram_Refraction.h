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
		glm::mat4 modelToWorldMatrix = glState->modelToWorldMatrix;
		glm::mat4 worldToCameraMatrix = glState->worldToCameraMatrix;
		glm::mat4 modelToCameraMatrix = worldToCameraMatrix * modelToWorldMatrix;
		Material* material = glState->material;

		//Model to camera
		GLuint modelToCameraMatrixUniform = glGetUniformLocation(this->program, "modelToCameraMatrix");
		glUniformMatrix4fv(modelToCameraMatrixUniform, 1, GL_FALSE, glm::value_ptr(modelToCameraMatrix));

		//Normal model to camera
		glm::mat3 normalModelToCameraMatrix = glm::mat3(modelToCameraMatrix);
		normalModelToCameraMatrix = glm::transpose(glm::inverse(normalModelToCameraMatrix)); //Normal transformation
		GLuint normalModelToCameraMatrixUniform = glGetUniformLocation(this->program, "normalModelToCameraMatrix");
		glUniformMatrix3fv(normalModelToCameraMatrixUniform, 1, GL_FALSE, glm::value_ptr(normalModelToCameraMatrix));

		//position texture
		int positionTexture = glState->positionTextureUnit;
		GLuint positionTextureUniform = glGetUniformLocation(this->program, "positionTexture");
		glUniform1i(positionTextureUniform, positionTexture);

		//normal texture
		int normalTexture = glState->normalTextureUnit;
		GLuint normalTextureUniform = glGetUniformLocation(this->program, "normalTexture");
		glUniform1i(normalTextureUniform, normalTexture);

		//depth texture
		int depthTexture = glState->depthTextureUnit;
		GLuint depthTextureUniform = glGetUniformLocation(this->program, "depthTexture");
		glUniform1i(depthTextureUniform, depthTexture);

		//color buffer texture
		int colorBufferTexture = glState->colorBufferTextureUnit;
		GLuint colorBufferTextureUniform = glGetUniformLocation(this->program, "colorBufferTexture");
		glUniform1i(colorBufferTextureUniform, colorBufferTexture);

		//refractivity
		float refractivity = material->refractivity;
		GLuint refractivityUniform = glGetUniformLocation(this->program, "refractivity");
		glUniform1f(refractivityUniform, refractivity);

		//Refractive index
		float refractiveIndex = material->refractiveIndex;
		GLuint refractiveIndexUniform = glGetUniformLocation(this->program, "refractiveIndex");
		glUniform1f(refractiveIndexUniform, refractiveIndex);
	}
};