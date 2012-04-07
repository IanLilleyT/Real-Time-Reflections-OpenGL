#pragma once

#include <glm/glm.hpp> //For math

#include <tinyxml/tinyxml.h> //For parsing material file

#include "Utils/Utils.h" //For parsing glm types

class Material
{
public:
	//Construct/Destruct
	Material();
	~Material();
	
	//Initialize
	void initialize(TiXmlElement* element);

	//Diffuse Color
	void setDiffuseColor(glm::vec4 diffuseColor);
	glm::vec4 getDiffuseColor();

	//Specular Color
	void setSpecularColor(glm::vec4 specularColor);
	glm::vec4 getSpecularColor();

	//Specular Shininess
	void setSpecularShininess(float specularShininess);
	float getSpecularShininess();

	//Reflectivity
	void setReflectivity(float reflectivity);
	float getReflectivity();

	//Refractiviy
	void setRefractivity(float refractivity);
	float getRefractivity();

	//Transparency
	void setTransparency(float transparency);
	float getTransparency();

	//Name
	void setName(std::string name);
	std::string getName();

private:
	glm::vec4 diffuseColor;
	glm::vec4 specularColor;
	float specularShininess;
	float reflectivity;
	float refractivity;
	float transparency;
	std::string name;
};
