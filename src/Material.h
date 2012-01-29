#pragma once

#include <vector>
#include "glm/glm.hpp"

class Material
{
public:
	Material();
	~Material();
	
	glm::vec4 diffuseColor;
	glm::vec4 specularColor;
	float specularShininess;
	float reflectivity;
	int ID;

	//Static
	static void addMaterial(Material* material);
	static std::vector<Material*> getAllMaterials();
	
private:

	//Static
	static std::vector<Material*> allMaterials;
};
