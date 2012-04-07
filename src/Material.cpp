#include "Material.h"

Material::Material()
{
	this->diffuseColor = glm::vec4(1,1,1,1);
	this->specularColor = glm::vec4(1,1,1,1);
	this->specularShininess = .1f;
	this->reflectivity = 0.0f;
	this->refractivity = 1.0f;
	this->transparency = 0.0f;
}
Material::~Material(){}

//Initialize
void Material::initialize(TiXmlElement* element)
{
	//name
	TiXmlElement* nameElement = element->FirstChildElement("name");
	if(nameElement) this->setName(nameElement->FirstChild()->Value());

	//diffuse color
	TiXmlElement* diffuseColorElement = element->FirstChildElement("diffuseColor");
	if(diffuseColorElement) this->setDiffuseColor(Utils::parseIntoVec4(diffuseColorElement->FirstChild()->Value()));

	//specular color
	TiXmlElement* specularColorElement = element->FirstChildElement("specularColor");
	if(specularColorElement) this->setSpecularColor(Utils::parseIntoVec4(specularColorElement->FirstChild()->Value()));

	//specular shininess
	TiXmlElement* specularShininessElement = element->FirstChildElement("specularShininess");
	if(specularShininessElement) this->setSpecularShininess(Utils::parseIntoFloat(specularShininessElement->FirstChild()->Value()));

	//reflectivity
	TiXmlElement* reflectivityElement = element->FirstChildElement("reflectivity");
	if(reflectivityElement) this->setReflectivity(Utils::parseIntoFloat(reflectivityElement->FirstChild()->Value()));

	//refractivity
	TiXmlElement* refractivityElement = element->FirstChildElement("refractivity");
	if(refractivityElement) this->setRefractivity(Utils::parseIntoFloat(refractivityElement->FirstChild()->Value()));

	//transparency
	TiXmlElement* transparencyElement = element->FirstChildElement("transparency");
	if(transparencyElement) this->setTransparency(Utils::parseIntoFloat(transparencyElement->FirstChild()->Value()));
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

//Refractivity
void Material::setRefractivity(float refractivity)
{
	this->refractivity = refractivity;
}
float Material::getRefractivity()
{
	return this->refractivity;
}

//Transparency
void Material::setTransparency(float transparency)
{
	this->transparency = transparency;
}
float Material::getTransparency()
{
	return this->transparency;
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