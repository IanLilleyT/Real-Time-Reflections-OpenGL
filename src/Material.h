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

	//Name
	void setName(std::string name);
	std::string getName();
	
	//Public variables
	glm::vec4 diffuseColor;
	glm::vec4 specularColor;
	float specularShininess;
	float reflectivity;
	float reflectiveScatter;
	float refractivity;
	float refractiveIndex;
	float transparency;

private:
	std::string name;
};
