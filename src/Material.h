#pragma once

#include <vector>
#include <glm/glm.hpp>

class Material
{
friend class MaterialDatabase;
public:
	Material();
	~Material();
	
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

	//Alpha
	void setAlpha(float alpha);
	float getAlpha();

private:
	glm::vec4 diffuseColor;
	glm::vec4 specularColor;
	float specularShininess;
	float reflectivity;
	float alpha;
};
