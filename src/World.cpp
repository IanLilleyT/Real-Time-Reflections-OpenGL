#include "World.h"

World::World()
{
	this->objectMap["None"] = std::vector<Object*>();
}
World::~World(){}

void World::initialize(TiXmlElement* element)
{
	Factory* factory = Singleton<Factory>::Instance();
	TiXmlElement* objectElement = element->FirstChildElement("object");
	for(; objectElement != 0; objectElement = objectElement->NextSiblingElement("object"))
	{
		std::string objectType = objectElement->Attribute("type");
		Object* object = (Object*)factory->construct(objectType);
		object->initialize(objectElement);
		this->addObject(object);
	}
}

void World::update()
{
	std::vector<Object*>& lights = this->getObjectsByType("Light");
	Singleton<GLState>::Instance()->setLights(lights);

	//Update all objects
	std::map<std::string,std::vector<Object*>>::iterator iter;
	for(iter = this->objectMap.begin(); iter != this->objectMap.end(); ++iter)
	{
		std::string type = iter->first;
		std::vector<Object*>& objects = this->getObjectsByType(type);
		for(unsigned int i = 0; i < objects.size(); i++)
		{
			Object* object = objects.at(i);
			object->update();
		}
	}
}

//Objects
void World::addObject(Object* object)
{
	if(object != 0)
		(this->objectMap[object->getType()]).push_back(object);
}
void World::removeObject(Object* object)
{
	if(object != 0)
	{
		std::vector<Object*>& objects = this->getObjectsByType(object->getType());
		std::vector<Object*>::iterator iter = std::find(objects.begin(),objects.end(),object);
		if(iter != objects.end())
			objects.erase(iter);
	}
}
void World::removeObject(std::string name)
{
	this->removeObject(this->getObjectByName(name));
}

Object* World::getObjectByName(std::string name)
{
	std::map<std::string,std::vector<Object*>>::iterator iter;
	for(iter = this->objectMap.begin(); iter != this->objectMap.end(); ++iter)
	{
		std::string type = iter->first;
		Object* object = this->getObjectByTypeAndName(type, name);
		if(object != 0)
			return object;
	}
	return 0;
}
Object* World::getObjectByTypeAndName(std::string type, std::string name)
{
	std::vector<Object*> objects = this->getObjectsByType(type);
	for(unsigned int i = 0; i < objects.size(); i++)
	{
		Object* object = objects.at(i);
		std::string currName = object->getName();
		if(currName == name)
			return object;
	}
	return 0;
}
std::vector<Object*>& World::getObjectsByType(std::string type)
{
	std::map<std::string,std::vector<Object*>>::iterator iter;
	iter = this->objectMap.find(type);
	if(iter != this->objectMap.end())
		return iter->second;
	else
		return this->objectMap["None"];
}