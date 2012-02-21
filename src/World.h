#pragma once

#include <algorithm>
#include <vector>
#include <map>
#include <string>

#include <tinyxml/tinyxml.h>

#include "Singleton.h"
#include "OpenGL/GLState.h"
#include "Objects/Object.h"
#include "Objects/Light.h"
#include "ObjectFactory.h"

class World
{
public:
	World();
	~World();

	void initialize(TiXmlElement* element);

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