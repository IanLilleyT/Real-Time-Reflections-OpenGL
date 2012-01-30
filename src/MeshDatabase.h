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

#include "Mesh.h"
#include "Utils.h"

class MeshDatabase
{
public:
	MeshDatabase();
	~MeshDatabase();

	Mesh* loadMesh(std::string meshFileName);

private:
	Mesh* findMesh(std::string meshFileName);
	Mesh* openMeshFile(std::string meshFileName);
	std::map<std::string,Mesh*> meshMap;
};