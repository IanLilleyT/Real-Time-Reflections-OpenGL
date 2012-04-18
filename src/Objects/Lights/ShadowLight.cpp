#include "ShadowLight.h"

//Static vars
std::string ShadowLight::className = "ShadowLight";

ShadowLight::ShadowLight() : Light() 
{
	this->lightCamera = new Camera3rdPerson();
}
ShadowLight::~ShadowLight(){}

//Initialize
void ShadowLight::initialize(TiXmlElement* element)
{
	Light::initialize(element);
	this->initialize();
}
void ShadowLight::initialize(std::string name, glm::vec4 intensity)
{
	Light::initialize(name,intensity);
	this->initialize();
}
void ShadowLight::initialize()
{
	//Nothing
}

//Type
std::string ShadowLight::getType()
{
	return ShadowLight::className;
}

//Transformations
void ShadowLight::updateTransformationMatrix()
{
	Light::updateTransformationMatrix();
	this->lightCamera->setCameraPos(this->getTranslation());
	glm::mat4 worldToCameraMatrix = this->lightCamera->getWorldToCameraMatrix();
	Singleton<GLState>::Instance()->shadowLightWorldToCameraMatrix = worldToCameraMatrix;
}