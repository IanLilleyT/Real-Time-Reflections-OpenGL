#pragma once

#include "GLProgram.h"

class GLProgram_DeferredLightingPass: public GLProgram
{
public:
	GLProgram_DeferredLightingPass() : GLProgram(){}
	virtual ~GLProgram_DeferredLightingPass(){}

protected:
	virtual void bindUniformBlocks()
	{
		this->bindUniformBlock(GLUniformBlockHelper::TYPE_PROJECTION);
		this->bindUniformBlock(GLUniformBlockHelper::TYPE_LIGHT);
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

		//position texture
		int positionTexture = glState->positionTextureUnit;
		GLuint positionTextureUniform = glGetUniformLocation(this->program, "positionTexture");
		glUniform1i(positionTextureUniform, positionTexture);

		//normal texture
		int normalTexture = glState->normalTextureUnit;
		GLuint normalTextureUniform = glGetUniformLocation(this->program, "normalTexture");
		glUniform1i(normalTextureUniform, normalTexture);

		//diffuseColor texture
		int diffuseColorTexture = glState->diffuseColorTextureUnit;
		GLuint diffuseColorTextureUniform = glGetUniformLocation(this->program, "diffuseColorTexture");
		glUniform1i(diffuseColorTextureUniform, diffuseColorTexture);

		//specularColor texture
		int specularColorTexture = glState->specularColorTextureUnit;
		GLuint specularColorTextureUniform = glGetUniformLocation(this->program, "specularColorTexture");
		glUniform1i(specularColorTextureUniform, specularColorTexture);

		//depth texture
		int depthTexture = glState->depthTextureUnit;
		GLuint depthTextureUniform = glGetUniformLocation(this->program, "depthTexture");
		glUniform1i(depthTextureUniform, depthTexture);

		//Light stuff (add support for more later)
		glm::vec3 lightCameraSpacePosition = glState->lightCameraSpacePosition;
		GLuint lightCameraSpacePositionUniform = glGetUniformLocation(this->program, "lightCameraSpacePosition");
		glUniform3fv(lightCameraSpacePositionUniform, 1, glm::value_ptr(lightCameraSpacePosition));

		glm::vec3 lightColor = glState->lightColor;
		GLuint lightColorUniform = glGetUniformLocation(this->program,"lightColor");
		glUniform3fv(lightColorUniform, 1, glm::value_ptr(lightColor));

		float lightIntensity = glState->lightIntensity;
		GLuint lightIntensityUniform = glGetUniformLocation(this->program,"lightIntensity");
		glUniform1f(lightIntensityUniform,lightIntensity);
	}
};