#include "Material.h"

Material::Material()
{
	this->diffuseColor = glm::vec4(1,1,1,1);
	this->specularColor = glm::vec4(1,1,1,1);
	this->specularShininess = .1f;
	this->reflectivity = 0.0f;
	this->reflectivityScatter = 0.0f;
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
	if(diffuseColorElement) this->diffuseColor = Utils::parseIntoVec4(diffuseColorElement->FirstChild()->Value());

	//specular color
	TiXmlElement* specularColorElement = element->FirstChildElement("specularColor");
	if(specularColorElement) this->specularColor = Utils::parseIntoVec4(specularColorElement->FirstChild()->Value());

	//specular shininess
	TiXmlElement* specularShininessElement = element->FirstChildElement("specularShininess");
	if(specularShininessElement) this->specularShininess = Utils::parseIntoFloat(specularShininessElement->FirstChild()->Value());

	//reflectivity
	TiXmlElement* reflectivityElement = element->FirstChildElement("reflectivity");
	if(reflectivityElement) this->reflectivity = Utils::parseIntoFloat(reflectivityElement->FirstChild()->Value());

	//reflectivity scatter
	TiXmlElement* reflectivityScatterElement = element->FirstChildElement("reflectivityScatter");
	if(reflectivityScatterElement) this->reflectivityScatter = Utils::parseIntoFloat(reflectivityScatterElement->FirstChild()->Value());

	//refractivity
	TiXmlElement* refractivityElement = element->FirstChildElement("refractivity");
	if(refractivityElement) this->refractivity = Utils::parseIntoFloat(refractivityElement->FirstChild()->Value());

	//transparency
	TiXmlElement* transparencyElement = element->FirstChildElement("transparency");
	if(transparencyElement) this->transparency = Utils::parseIntoFloat(transparencyElement->FirstChild()->Value());
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