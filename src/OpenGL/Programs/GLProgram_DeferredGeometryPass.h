#pragma once

#include "GLProgram.h"

class GLProgram_DeferredGeometryPass: public GLProgram
{
public:
	GLProgram_DeferredGeometryPass() : GLProgram(){}
	virtual ~GLProgram_DeferredGeometryPass(){}

protected:
	virtual void bindUniformBlocks()
	{
		GLProgram::bindUniformBlocks();
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

		//In Diffuse Color
		glm::vec4 diffuseColor = material->diffuseColor;
		GLuint diffuseColorUniform = glGetUniformLocation(this->program, "diffuseColor");
		glUniform4fv(diffuseColorUniform, 1, glm::value_ptr(diffuseColor));

		//Specular Color
		glm::vec4 specularColor = material->specularColor;
		GLuint specularColorUniform = glGetUniformLocation(this->program, "specularColor");
		glUniform4fv(specularColorUniform, 1, glm::value_ptr(specularColor));

		//Specular Shininess
		float specularShininess = material->specularShininess;
		GLuint specularShininessUniform = glGetUniformLocation(this->program, "specularShininess");
		glUniform1f(specularShininessUniform, specularShininess);

		//Roughness
		float roughness = material->roughness;
		GLuint roughnessUniform = glGetUniformLocation(this->program, "roughness");
		glUniform1f(roughnessUniform, roughness);

		//Reflectivity
		float reflectivity = material->reflectivity;
		GLuint reflectivityUniform = glGetUniformLocation(this->program, "reflectivity");
		glUniform1f(reflectivityUniform,reflectivity);

		//Refractivity
		float refractivity = material->refractivity;
		GLuint refractivityUniform = glGetUniformLocation(this->program, "refractivity");
		glUniform1f(refractivityUniform, refractivity);

		//Refractive index
		float refractiveIndex = material->refractiveIndex;
		GLuint refractiveIndexUniform = glGetUniformLocation(this->program, "refractiveIndex");
		glUniform1f(refractiveIndexUniform, refractiveIndex);
	}
};