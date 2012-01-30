#include "Material.h"

Material::Material()
{
	//Do nothing
}
Material::~Material()
{
	//Do nothing
};

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