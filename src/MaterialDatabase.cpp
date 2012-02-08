#include "MaterialDatabase.h"

MaterialDatabase::MaterialDatabase(){}
MaterialDatabase::~MaterialDatabase(){}

Material* MaterialDatabase::loadMaterial(std::string materialFileName)
{
	Material* newMaterial = 0;

	std::string extension = ".mtl";
	if(materialFileName.find(extension) == std::string::npos)
		materialFileName += extension;

	newMaterial = this->findMaterial(materialFileName);
	if(newMaterial == 0) 
	{
		newMaterial = this->openMaterialFile(materialFileName);
		if(newMaterial != 0)
			this->materialMap[materialFileName] = newMaterial;
	}
	return newMaterial;
}
Material* MaterialDatabase::findMaterial(std::string materialFileName)
{
	std::map<std::string, Material*>::iterator it;
	it = this->materialMap.find(materialFileName);
	if(it != this->materialMap.end())
		return it->second;
	else
		return 0;
}
Material* MaterialDatabase::openMaterialFile(std::string materialFileName)
{
	std::string materialPath = Singleton<GlobalValues>::Instance()->getMaterialPath();
	std::string fullPath = materialPath + materialFileName;

	std::string line;
	std::ifstream file;

	file.open(fullPath.c_str(), std::ifstream::in);
	if(file.is_open())
	{
		Material* material = new Material();
		while(file.good())
		{
			//Diffuse Color
			getline(file, line);
			glm::vec4 diffuseColor = Utils::parseIntoVec4(line);
			material->setDiffuseColor(diffuseColor);

			//Specular Color
			getline(file, line);
			glm::vec4 specularColor = Utils::parseIntoVec4(line);
			material->setSpecularColor(specularColor);
			
			//Specular Shininess
			getline(file, line);
			float specularShininess = Utils::parseIntoFloat(line);
			material->setSpecularShininess(specularShininess);

			//Reflectivity
			getline(file, line);
			float reflectivity = Utils::parseIntoFloat(line);
			material->setReflectivity(reflectivity);

			//Alpha
			getline(file, line);
			float alpha = Utils::parseIntoFloat(line);
			material->setAlpha(alpha);
		}
		file.close();
		return material;
	}
	else
	{
		std::cout << "Unable to open world file." << std::endl;
		return 0;
	}
}
