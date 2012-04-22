#include "Material.h"

Material::Material()
{
	this->diffuseColor = glm::vec4(1,1,1,1);
	this->specularColor = glm::vec4(1,1,1,1);
	this->specularShininess = .1f;
	this->roughness = 0.0f;
	this->reflectivity = 0.0f;
	this->refractivity = 0.0f;
	this->refractiveIndex = 1.0f;
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
	if(specularShininessElement) this->specularShininess = Utils::convertStringToFloat(specularShininessElement->FirstChild()->Value());

	//roughness
	TiXmlElement* roughnessElement = element->FirstChildElement("roughness");
	if(roughnessElement) this->roughness = Utils::convertStringToFloat(roughnessElement->FirstChild()->Value());

	//reflectivity
	TiXmlElement* reflectivityElement = element->FirstChildElement("reflectivity");
	if(reflectivityElement) this->reflectivity = Utils::convertStringToFloat(reflectivityElement->FirstChild()->Value());

	//refractivity
	TiXmlElement* refractivityElement = element->FirstChildElement("refractivity");
	if(refractivityElement) this->refractivity = Utils::convertStringToFloat(refractivityElement->FirstChild()->Value());

	//refractive index
	TiXmlElement* refractiveIndexElement = element->FirstChildElement("refractiveIndex");
	if(refractiveIndexElement) this->refractiveIndex = Utils::convertStringToFloat(refractiveIndexElement->FirstChild()->Value());

	//transparency
	TiXmlElement* transparencyElement = element->FirstChildElement("transparency");
	if(transparencyElement) this->transparency = Utils::convertStringToFloat(transparencyElement->FirstChild()->Value());
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