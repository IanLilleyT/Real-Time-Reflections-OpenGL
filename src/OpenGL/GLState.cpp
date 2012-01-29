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

//WorldToCameraMatrix
void GLState::setWorldToCameraMatrix(glm::mat4 worldToCameraMatrix)
{
	this->worldToCameraMatrix = worldToCameraMatrix;
}
glm::mat4 GLState::getWorldToCameraMatrix()
{
	return this->worldToCameraMatrix;
}

//CameraToClipMatrix
void GLState::setCameraToClipMatrix(glm::mat4 cameraToClipMatrix)
{
	this->cameraToClipMatrix = cameraToClipMatrix;
}
glm::mat4 GLState::getCameraToClipMatrix()
{
	return this->cameraToClipMatrix;
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
void GLState::setLights(std::vector<Light*> lights)
{
	this->lights = lights;
}
std::vector<Light*> GLState::getLights()
{
	return this->lights;
}