#include "Light.h"

//Static vars
std::string Light::className = "Light";

glm::vec4 Light::ambientIntensity = glm::vec4(0.0f,0.0f,0.0f,1.0f);
float Light::lightAttenuation = 0.1f;
float Light::maxIntensity = 1.0f;
float Light::gamma = 2.2f;

Light::Light() : Object() {}
Light::~Light(){}

//Initialize
void Light::initialize(TiXmlElement* element)
{
	glm::vec4 color = glm::vec4(1,1,1,1);
	TiXmlElement* colorElement = element->FirstChildElement("color");
	if(colorElement) color = Utils::parseIntoVec4(colorElement->FirstChild()->Value());

	float intensity = 1.0f;
	TiXmlElement* intensityElement = element->FirstChildElement("intensity");
	if(intensityElement) intensity = Utils::convertStringToFloat(intensityElement->FirstChild()->Value());
	
	this->initialize(color,intensity);
	Object::initialize(element);
}
void Light::initialize(std::string name, glm::vec4 color, float intensity)
{
	this->initialize(color,intensity);
	Object::initialize(name);
}
void Light::initialize(glm::vec4 color, float intensity)
{
	this->setColor(color);
	this->setIntensity(intensity);
}

//Type
std::string Light::getClassname()
{
	return Light::className;
}

//Color
void Light::setColor(glm::vec4 color)
{
	this->color = color;
}
glm::vec4 Light::getColor()
{
	return this->color;
}

//Intensity
void Light::setIntensity(float intensity)
{
	this->intensity = intensity;
}
float Light::getIntensity()
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