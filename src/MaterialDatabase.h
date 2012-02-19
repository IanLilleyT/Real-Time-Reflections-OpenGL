#pragma once

#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <istream>
#include <string>
#include <cstdlib>
#include <utility>
#include <limits.h>
#include <iterator>
#include <map>
#include <limits>
#include <cfloat>

#include <glm/glm.hpp>

#include "Material.h"
#include "GlobalPaths.h"
#include "Singleton.h"
#include "Utils.h"

class MaterialDatabase
{
public:
	MaterialDatabase();
	~MaterialDatabase();

	Material* loadMaterial(std::string materialFileName);
	
private:
	Material* findMaterial(std::string materialFileName);
	Material* openMaterialFile(std::string materialFileName);
	std::map<std::string, Material*> materialMap;

	bool isFieldValid(std::ifstream& file, std::string name, std::vector<std::string>& results);
};