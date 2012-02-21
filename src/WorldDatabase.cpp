#include "WorldDatabase.h"

WorldDatabase::WorldDatabase(){}
WorldDatabase::~WorldDatabase(){}

World* WorldDatabase::loadWorld(std::string worldFileName)
{
	World* newWorld = 0;

	newWorld = this->findWorld(worldFileName);
	if(newWorld == 0) 
	{
		newWorld = this->openWorldFile(worldFileName);
		if(newWorld != 0)
			this->worldMap[worldFileName] = newWorld;
	}
	return newWorld;
}
World* WorldDatabase::findWorld(std::string worldFileName)
{
	std::map<std::string, World*>::iterator it;
	it = this->worldMap.find(worldFileName);
	if(it != this->worldMap.end())
		return it->second;
	else
		return 0;
}
World* WorldDatabase::openWorldFile(std::string worldFileName)
{
	worldFileName += ".xml";
	std::string worldPath = Singleton<GlobalPaths>::Instance()->getWorldPath();
	std::string fullPath = worldPath + worldFileName;

	ObjectFactory* objectFactory = Singleton<ObjectFactory>::Instance();
	World* world = new World();

	//XML loading
	TiXmlDocument doc;
	doc.LoadFile(fullPath.c_str());
	if(doc.Error()) throw std::runtime_error(doc.ErrorDesc());
	TiXmlHandle docHandle(&doc);
	TiXmlElement* worldElement = docHandle.FirstChild("world").ToElement();
	world->initialize(worldElement);

	return world;
}