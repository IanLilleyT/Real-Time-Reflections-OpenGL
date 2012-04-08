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
		glm::vec4 diffuseColor = material->diffuseColor;
		GLuint diffuseColorUniform = glGetUniformLocation(this->program, "diffuseColor");
		glUniform4fv(diffuseColorUniform, 1, glm::value_ptr(diffuseColor));

		//Specular Color
		glm::vec4 specularColor = material->specularColor;
		GLuint specularColorUniform = glGetUniformLocation(this->program, "specularColor");
		glUniform4fv(specularColorUniform, 1, glm::value_ptr(specularColor));

		//Specular Shininess
		float shininessFactor = material->specularShininess;
		GLuint shininessFactorUniform = glGetUniformLocation(this->program, "specularShininess");
		glUniform1f(shininessFactorUniform, shininessFactor);

		//Transparency
		float transparency = material->transparency;
		GLuint transparencyUniform = glGetUniformLocation(this->program, "transparency");
		glUniform1f(transparencyUniform,transparency);

		//Reflectivity
		float reflectivity = material->reflectivity;
		GLuint reflectivityUniform = glGetUniformLocation(this->program, "reflectivity");
		glUniform1f(reflectivityUniform,reflectivity);

		//Reflectivity scatter
		float reflectivityScatter = material->reflectivityScatter;
		GLuint reflectivityScatterUniform = glGetUniformLocation(this->program, "reflectivityScatter");
		glUniform1f(reflectivityScatterUniform, reflectivityScatter);

		//Refractivity
		float refractivity = material->refractivity;
		GLuint refractivityUniform = glGetUniformLocation(this->program, "refractivity");
		glUniform1f(refractivityUniform, refractivity);

		//Color texture front
		GLuint colorTextureFrontUniform = glGetUniformLocation(this->program, "colorTextureFront");
		glUniform1i(colorTextureFrontUniform, 0);

		//Depth texture front
		GLuint depthTextureFrontUniform = glGetUniformLocation(this->program, "depthTextureFront");
		glUniform1i(depthTextureFrontUniform, 1);

		//Color texture back
		GLuint colorTextureBackUniform = glGetUniformLocation(this->program, "colorTextureBack");
		glUniform1i(colorTextureBackUniform, 2);

		//Depth texture back
		GLuint depthTextureBackUniform = glGetUniformLocation(this->program, "depthTextureBack");
		glUniform1i(depthTextureBackUniform, 3);

		//Num Lights
		int numLights = glState->getLights().size();
		GLuint numLightsUniform = glGetUniformLocation(this->program, "numLights");
		glUniform1i(numLightsUniform, numLights);
	}
};