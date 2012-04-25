#include "ShadowLight.h"

//Static vars
std::string ShadowLight::className = "ShadowLight";

ShadowLight::ShadowLight() : Light() {}
ShadowLight::~ShadowLight(){}

//Initialize
void ShadowLight::initialize(TiXmlElement* element)
{
	this->initialize();
	Light::initialize(element);
}
void ShadowLight::initialize(std::string name, glm::vec4 intensity)
{
	this->initialize();
	Light::initialize(name,intensity);
}
void ShadowLight::initialize()
{
	this->lightCamera = new Camera3rdPerson();
}

//Type
std::string ShadowLight::getClassname()
{
	return ShadowLight::className;
}

//Transformations
void ShadowLight::updateTransformationMatrix()
{
	Light::updateTransformationMatrix();
	this->lightCamera->setCameraPos(this->getTranslation());
}

//World to camera matrix
glm::mat4 ShadowLight::getWorldToCameraMatrix()
{
	return this->lightCamera->getWorldToCameraMatrix();
}