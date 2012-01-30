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
#include "Mesh.h"
#include "MeshDatabase.h"
#include "MaterialDatabase.h"
#include "Objects/RenderObject.h"
#include "Objects/Light.h"
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
};