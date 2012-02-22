#pragma once

#include <algorithm> //For finding object in array
#include <vector> //For storing objects
#include <map> //For storing object
#include <string> //For object names

#include <tinyxml/tinyxml.h> //For parsing world xml file

#include "Singleton.h" //For using global classes
#include "Objects/Object.h" //Storage type
#include "OpenGL/GLState.h" //Takes the world's lights
#include "Objects/Light.h" //For sending lights to GLState
#include "Factory.h" //For constructing new objects during initialization

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