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

//Effect type
void GLState::setEffectType(int effectType)
{
	this->effectType = effectType;
}
int GLState::getEffectType()
{
	return this->effectType;
}

//Reflection textures
void GLState::setReflectionTextures(int colorFront, int depthFront, int colorBack, int depthBack)
{
	this->colorTextureFront = colorFront;
	this->depthTextureFront = depthFront;
	this->colorTextureBack = colorBack;
	this->depthTextureBack = depthBack;
}