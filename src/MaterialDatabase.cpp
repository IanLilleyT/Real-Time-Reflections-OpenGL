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

	std::ifstream file;
	std::vector<std::string> results;

	file.open(fullPath.c_str(), std::ifstream::in);
	if(file.is_open())
	{
		Material* material = new Material();
		while(file.good())
		{
			//Diffuse Color
			if(this->isFieldValid(file,"diffuseColor",results))
			{
				glm::vec4 diffuseColor = Utils::parseIntoVec4(results);
				material->setDiffuseColor(diffuseColor);
			}

			//Specular Color
			if(this->isFieldValid(file,"specularColor",results))
			{
				glm::vec4 specularColor = Utils::parseIntoVec4(results);
				material->setSpecularColor(specularColor);
			}
			
			//Specular Shininess
			if(this->isFieldValid(file,"specularShininess",results))
			{
				float specularShininess = Utils::parseIntoFloat(results);
				material->setSpecularShininess(specularShininess);
			}

			//Reflectivity
			if(this->isFieldValid(file,"reflectivity",results))
			{
				float reflectivity = Utils::parseIntoFloat(results);
				material->setReflectivity(reflectivity);
			}

			//Alpha
			if(this->isFieldValid(file,"alpha",results))
			{
				float alpha = Utils::parseIntoFloat(results);
				material->setAlpha(alpha);
			}

			//Name
			material->setName(materialFileName);
		}
		file.close();
		return material;
	}
	else
	{
		std::cout << "Unable to open material file." << std::endl;
		return 0;
	}
}

bool MaterialDatabase::isFieldValid(std::ifstream& file, std::string name, std::vector<std::string>& results)
{
	std::string line;
	getline(file, line);
	results = Utils::splitByCharacter(line, ' ');
	return results.at(0) == name;
}