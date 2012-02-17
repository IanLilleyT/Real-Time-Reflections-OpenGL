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

#include "World.h"
#include "MeshDatabase.h"
#include "MaterialDatabase.h"
#include "Objects/RenderObject.h"
#include "Objects/Light.h"
#include "Objects/Jello.h"
#include "Objects/Primitives/Sphere.h"
#include "Singleton.h"
#include "GlobalValues.h"
#include "OpenGL/GLProgramDatabase.h"

class WorldDatabase
{
public:
	WorldDatabase();
	~WorldDatabase();

	World* loadWorld(std::string worldFileName);

private:
	World* findWorld(std::string worldFileName);
	World* openWorldFile(std::string worldFileName);
	std::map<std::string, World*> worldMap;

	bool isFieldValid(std::ifstream& file, std::string name, std::vector<std::string>& results);

	void processRenderObject(std::ifstream& file, World* world);
	void processSphere(std::ifstream& file, World* world);
	void processJello(std::ifstream& file, World* world);
	void processLight(std::ifstream& file, World* world);
};