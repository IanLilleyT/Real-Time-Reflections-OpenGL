#pragma once

#include <algorithm>
#include <vector>
#include <map>
#include <string>

#include "Singleton.h"
#include "OpenGL/GLState.h"
#include "Objects/Object.h"
#include "Objects/RenderObject.h"
#include "Objects/Light.h"

class World
{
friend class WorldDatabase;
public:
	World();
	~World();

	void update();

	void addObject(Object* object);
	void removeObject(Object* object);
	void removeObject(std::string name);

	std::vector<Object*>& getObjectsByType(std::string type);
	Object* getObjectByTypeAndName(std::string type, std::string name);
	Object* getObjectByName(std::string name);

private:
	
	std::map<std::string,std::vector<Object*>> objectMap;
};