#pragma once

#include <string>
#include <map>

#include "World.h"
#include "Singleton.h"
#include "GlobalPaths.h"

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