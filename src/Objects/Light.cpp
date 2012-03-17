#include "Light.h"

//Static vars
std::string Light::className = "Light";

glm::vec4 Light::ambientIntensity = glm::vec4(0.2f,0.2f,0.2f,1.0f);
float Light::lightAttenuation = 0.1f;
float Light::maxIntensity = 1.0f;
float Light::gamma = 2.2f;

Light::Light() : Object() {}
Light::~Light(){}

//Initialize
void Light::initialize(TiXmlElement* element)
{
	Object::initialize(element);

	std::string intensityString = element->FirstChildElement("intensity")->FirstChild()->Value();
	glm::vec4 intensityVal = Utils::parseIntoVec4(intensityString);

	this->initialize(intensityVal);
}
void Light::initialize(std::string name, glm::vec4 intensity)
{
	Object::initialize(name);
	this->initialize(intensity);
}
void Light::initialize(glm::vec4 intensity)
{
	this->setIntensity(intensity);
}

//Type
std::string Light::getType()
{
	return Light::className;
}

//Intensity
void Light::setIntensity(glm::vec4 intensity)
{
	this->intensity = intensity;
}
glm::vec4 Light::getIntensity()
{
	return this->intensity;
}

/*-----------------------
  Statics
-----------------------*/
//Setters
void Light::setAmbientIntensity(glm::vec4 ambientIntensity)
{
	Light::ambientIntensity = ambientIntensity;	
}
void Light::setLightAttenuation(float lightAttenuation)
{
	Light::lightAttenuation = lightAttenuation;
}
void Light::setMaxIntensity(float maxIntensity)
{
	Light::maxIntensity = maxIntensity;
}
void Light::setGamma(float gamma)
{
	Light::gamma = gamma;
}

//Getters
glm::vec4 Light::getAmbientIntensity()
{
	return Light::ambientIntensity;
}
float Light::getLightAttenuation()
{
	return Light::lightAttenuation;
}
float Light::getMaxIntensity()
{
	return Light::maxIntensity;
}
float Light::getGamma()
{
	return Light::gamma;
}