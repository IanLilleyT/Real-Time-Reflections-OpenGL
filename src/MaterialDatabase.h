#pragma once

#include <string> //For names
#include <map> //For storing materials

#include "Material.h" //For storing materials
#include "GlobalPaths.h" //For getting material path
#include "Singleton.h" //For using global classes

class MaterialDatabase
{
public:
	MaterialDatabase();
	~MaterialDatabase();

	Material* loadMaterial(std::string materialFileName);
	
	static std::string NO_NAME;

private:
	Material* findMaterial(std::string materialFileName);
	Material* openMaterialFile(std::string materialFileName);
	std::map<std::string, Material*> materialMap;
};