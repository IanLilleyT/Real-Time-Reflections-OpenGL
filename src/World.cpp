#include "World.h"

World::World(){}
World::~World(){}

void World::initialize(TiXmlElement* element)
{
	this->setName(element->FirstChildElement("name")->FirstChild()->Value());
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
	//Update lights to GLState
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

void World::render()
{
	//Render all objects
	std::map<std::string,std::vector<Object*>>::iterator iter;
	for(iter = this->objectMap.begin(); iter != this->objectMap.end(); ++iter)
	{
		std::string type = iter->first;
		std::vector<Object*>& objects = this->getObjectsByType(type);
		for(unsigned int i = 0; i < objects.size(); i++)
		{
			Object* object = objects.at(i);
			object->render();
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
		Object* object = this->getObjectByNameAndType(type, name);
		if(object != 0)
			return object;
	}
	return 0;
}
Object* World::getObjectByNameAndType(std::string type, std::string name)
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
std::vector<Object*> World::getObjectsByType(std::string type, bool exclusive)
{
	std::vector<Object*> objects;

	if(exclusive)
	{
		std::map<std::string,std::vector<Object*>>::iterator iter;
		iter = this->objectMap.find(type);
		if(iter != this->objectMap.end())
			objects = iter->second;
	}
	else
	{
		//Loop over class hierarchy and accumulate objects from the subtree of type
		tree<std::string>::iterator iter = Object::classHierarchy.begin();
		int typeDepth = -1;

		while(iter != Object::classHierarchy.end())
		{
			int nodeDepth = tree<std::string>::depth(iter);
			std::string nodeName = *iter;
			++iter;

			if(nodeName == type)
				typeDepth = nodeDepth;

			//Only use nodes that are in the subtree of type
			if(typeDepth >= 0)
			{
				if(nodeDepth > typeDepth || nodeName == type)
				{
					std::vector<Object*> typeObjects = this->getObjectsByType(nodeName);
					objects.insert(objects.end(),typeObjects.begin(),typeObjects.end());
				}
				else break;
			}
		}
	}
	return objects;
}

//Name
void World::setName(std::string name)
{
	this->name = name;
}
std::string World::getName()
{
	return this->name;
}