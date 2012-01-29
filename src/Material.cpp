#include "Material.h"

std::vector<Material*> Material::allMaterials = std::vector<Material*>();

Material::Material()
{
	//Do nothing
}
Material::~Material()
{
	//Do nothing
};

//Static
void Material::addMaterial(Material* material)
{
	Material::allMaterials.push_back(material);
}
std::vector<Material*> Material::getAllMaterials()
{
	return Material::allMaterials;
}