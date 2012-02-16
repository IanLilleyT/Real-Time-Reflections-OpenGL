#include "Light.h"

//Default static values
glm::vec4 Light::ambientIntensity = glm::vec4(0.2f,0.2f,0.2f,1.0f);
float Light::lightAttenuation = 0.1f;
float Light::maxIntensity = 1.0f;
float Light::gamma = 2.2f;

std::string Light::className = "Light";
Light::Light() : Object() 
{
	this->type = Object::className;
}
Light::Light(std::string name, glm::vec4 intensity) : Object(name) 
{
	this->type = Light::className;
	this->setIntensity(intensity);
}
Light::~Light(){}

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