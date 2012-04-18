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
		this->bindUniformBlock(GLUniformBlockHelper::TYPE_EFFECT_TYPE);
	}
	virtual void fillUniforms()
	{
		GLState* glState = Singleton<GLState>::Instance();
		GLCamera* glCamera = Singleton<GLCamera>::Instance();
		glm::mat4 modelToWorldMatrix = glState->modelToWorldMatrix;
		glm::mat4 worldToCameraMatrix = glCamera->getWorldToCameraMatrix();
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
		float reflectiveScatter = material->reflectiveScatter;
		GLuint reflectiveScatterUniform = glGetUniformLocation(this->program, "reflectiveScatter");
		glUniform1f(reflectiveScatterUniform, reflectiveScatter);

		//Refractivity
		float refractivity = material->refractivity;
		GLuint refractivityUniform = glGetUniformLocation(this->program, "refractivity");
		glUniform1f(refractivityUniform, refractivity);

		//Refractive index
		float refractiveIndex = material->refractiveIndex;
		GLuint refractiveIndexUniform = glGetUniformLocation(this->program, "refractiveIndex");
		glUniform1f(refractiveIndexUniform, refractiveIndex);

		//Color texture front
		GLuint colorTextureFrontUniform = glGetUniformLocation(this->program, "colorTextureFront");
		glUniform1i(colorTextureFrontUniform, glState->colorTextureFront);

		//Depth texture front
		GLuint depthTextureFrontUniform = glGetUniformLocation(this->program, "depthTextureFront");
		glUniform1i(depthTextureFrontUniform, glState->depthTextureFront);

		//Color texture back
		GLuint colorTextureBackUniform = glGetUniformLocation(this->program, "colorTextureBack");
		glUniform1i(colorTextureBackUniform, glState->colorTextureBack);

		//Depth texture back
		GLuint depthTextureBackUniform = glGetUniformLocation(this->program, "depthTextureBack");
		glUniform1i(depthTextureBackUniform, glState->depthTextureBack);

		//Num Lights
		int numLights = glState->lights.size();
		GLuint numLightsUniform = glGetUniformLocation(this->program, "numLights");
		glUniform1i(numLightsUniform, numLights);
	}
};