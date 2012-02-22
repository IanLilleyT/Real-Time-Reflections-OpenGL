#include "MaterialDatabase.h"

std::string MaterialDatabase::NO_NAME = "NO_NAME";
MaterialDatabase::MaterialDatabase(){}
MaterialDatabase::~MaterialDatabase(){}

Material* MaterialDatabase::loadMaterial(std::string materialFileName)
{
	Material* newMaterial = 0;
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
	std::string materialPath = Singleton<GlobalPaths>::Instance()->getMaterialPath();
	std::string fullPath = materialPath + materialFileName + ".xml";

	Material* material = new Material();

	//XML loading
	TiXmlDocument doc;
	doc.LoadFile(fullPath.c_str());
	if(doc.Error()) throw std::runtime_error(doc.ErrorDesc());
	TiXmlHandle docHandle(&doc);
	TiXmlElement* materialElement = docHandle.FirstChild("material").ToElement();
	material->initialize(materialElement);
	
	return material;
}