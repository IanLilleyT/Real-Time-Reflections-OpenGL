#pragma once

#include "GLProgram.h"

class GLProgram_Material: public GLProgram
{
public:
	GLProgram_Material() : GLProgram(){}
	virtual ~GLProgram_Material(){}
	virtual void createAttributes()
	{
		GLProgram::createAttributes();
		this->addAttribute("position", 3, GL_FLOAT);
		this->addAttribute("normal", 3, GL_FLOAT);
	}

protected:
	virtual void bindUniformBlocks()
	{
		GLProgram::bindUniformBlocks();
		this->bindUniformBlock(GLUniformBlockHelper::TYPE_PROJECTION);
		this->bindUniformBlock(GLUniformBlockHelper::TYPE_LIGHTS);
		this->bindUniformBlock(GLUniformBlockHelper::TYPE_REFLECTION_TOGGLE);
	}
	virtual void fillUniforms()
	{
		GLState* glState = Singleton<GLState>::Instance();
		glm::mat4 modelToWorldMatrix = glState->getModelToWorldMatrix();
		glm::mat4 worldToCameraMatrix = glState->getWorldToCameraMatrix();
		glm::mat4 modelToCameraMatrix = worldToCameraMatrix * modelToWorldMatrix;
		Material* material = glState->getMaterial();

		//Model to camera
		GLuint modelToCameraMatrixUniform = glGetUniformLocation(this->program, "modelToCameraMatrix");
		glUniformMatrix4fv(modelToCameraMatrixUniform, 1, GL_FALSE, glm::value_ptr(modelToCameraMatrix));

		//Normal model to camera
		glm::mat3 normalModelToCameraMatrix = glm::mat3(modelToCameraMatrix);
		normalModelToCameraMatrix = glm::transpose(glm::inverse(normalModelToCameraMatrix)); //Normal transformation
		GLuint normalModelToCameraMatrixUniform = glGetUniformLocation(this->program, "normalModelToCameraMatrix");
		glUniformMatrix3fv(normalModelToCameraMatrixUniform, 1, GL_FALSE, glm::value_ptr(normalModelToCameraMatrix));

		//In Diffuse Color
		glm::vec4 diffuseColor = material->getDiffuseColor();
		GLuint diffuseColorUniform = glGetUniformLocation(this->program, "diffuseColor");
		glUniform4fv(diffuseColorUniform, 1, glm::value_ptr(diffuseColor));

		//Specular Color
		glm::vec4 specularColor = material->getSpecularColor();
		GLuint specularColorUniform = glGetUniformLocation(this->program, "specularColor");
		glUniform4fv(specularColorUniform, 1, glm::value_ptr(specularColor));

		//Specular Shininess
		float shininessFactor = material->getSpecularShininess();
		GLuint shininessFactorUniform = glGetUniformLocation(this->program, "specularShininess");
		glUniform1f(shininessFactorUniform, shininessFactor);

		//Alpha
		float alpha = material->getAlpha();
		GLuint alphaUniform = glGetUniformLocation(this->program, "alpha");
		glUniform1f(alphaUniform,alpha);

		//Reflectivity
		float reflectivity = material->getReflectivity();
		GLuint reflectivityUniform = glGetUniformLocation(this->program, "reflectivity");
		glUniform1f(reflectivityUniform,reflectivity);

		//Color buffer texture
		GLuint colorTextureUniform = glGetUniformLocation(this->program, "colorTexture");
		glUniform1i(colorTextureUniform, 0);

		//Depth buffer texture
		GLuint depthTextureUniform = glGetUniformLocation(this->program, "depthTexture");
		//glUniform1i(depthTextureUniform, 1);

		//Num Lights
		int numLights = glState->getLights().size();
		GLuint numLightsUniform = glGetUniformLocation(this->program, "numLights");
		glUniform1i(numLightsUniform, numLights);
	}
};