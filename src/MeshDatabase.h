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

#include "OpenGL/GLMesh.h"
#include "Utils.h"

class MeshDatabase
{
public:
	MeshDatabase();
	~MeshDatabase();

	GLMesh* loadMesh(std::string meshFileName);
	void copyMesh(std::string meshFileName, std::string newName);

	static std::string NO_NAME;

private:
	GLMesh* findMesh(std::string meshFileName);
	GLMesh* openMeshFile(std::string meshFileName);
	std::map<std::string,GLMesh*> meshMap;

	bool isFieldValid(std::ifstream& file, std::string name, std::vector<std::string>& results);
};