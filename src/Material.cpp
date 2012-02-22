#include "Material.h"

Material::Material(){}
Material::~Material(){}

//Initialize
void Material::initialize(TiXmlElement* element)
{
	std::string name = element->FirstChildElement("name")->FirstChild()->Value();
	glm::vec4 diffuseColor = Utils::parseIntoVec4(element->FirstChildElement("diffuseColor")->FirstChild()->Value());
	glm::vec4 specularColor = Utils::parseIntoVec4(element->FirstChildElement("specularColor")->FirstChild()->Value());
	float specularShininess = Utils::parseIntoFloat(element->FirstChildElement("specularShininess")->FirstChild()->Value());
	float reflectivity = Utils::parseIntoFloat(element->FirstChildElement("reflectivity")->FirstChild()->Value());
	float alpha = Utils::parseIntoFloat(element->FirstChildElement("alpha")->FirstChild()->Value());

	this->setName(name);
	this->setDiffuseColor(diffuseColor);
	this->setSpecularColor(specularColor);
	this->setSpecularShininess(specularShininess);
	this->setReflectivity(reflectivity);
	this->setAlpha(alpha);
}

//Diffuse Color
void Material::setDiffuseColor(glm::vec4 diffuseColor)
{
	this->diffuseColor = diffuseColor;
}
glm::vec4 Material::getDiffuseColor()
{
	return this->diffuseColor;
}

//Specular Color
void Material::setSpecularColor(glm::vec4 specularColor)
{
	this->specularColor = specularColor;
}
glm::vec4 Material::getSpecularColor()
{
	return this->specularColor;
}

//Specular Shininess
void Material::setSpecularShininess(float specularShininess)
{
	this->specularShininess = specularShininess;
}
float Material::getSpecularShininess()
{
	return this->specularShininess;
}

//Reflectivity
void Material::setReflectivity(float reflectivity)
{
	this->reflectivity = reflectivity;
}
float Material::getReflectivity()
{
	return this->reflectivity;
}

//Alpha
void Material::setAlpha(float alpha)
{
	this->alpha = alpha;
}
float Material::getAlpha()
{
	return this->alpha;
}

//Name
void Material::setName(std::string name)
{
	this->name = name;
}
std::string Material::getName()
{
	return this->name;
}