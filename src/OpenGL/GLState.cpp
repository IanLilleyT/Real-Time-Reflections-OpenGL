#include "GLState.h"

GLState::GLState(){};
GLState::~GLState(){};

//ModelToWorldMatrix
void GLState::setModelToWorldMatrix(glm::mat4 modelToWorldMatrix)
{
	this->modelToWorldMatrix = modelToWorldMatrix;
}
glm::mat4 GLState::getModelToWorldMatrix()
{
	return this->modelToWorldMatrix;
}

//Material
void GLState::setMaterial(Material* material)
{
	this->material = material;
}
Material* GLState::getMaterial()
{
	return this->material;
}

//Lights
void GLState::setLights(std::vector<Object*>& lights)
{
	this->lights = lights;
}
std::vector<Object*>& GLState::getLights()
{
	return this->lights;
}

//Reflection toggle
void GLState::setReflectionToggle(int reflectionToggle)
{
	this->reflectionToggle = reflectionToggle;
}
int GLState::getReflectionToggle()
{
	return this->reflectionToggle;
}