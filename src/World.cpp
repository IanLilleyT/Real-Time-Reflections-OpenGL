#include "World.h"

World::World(){}
World::~World(){}

void World::update()
{
	Singleton<GLState>::Instance()->setLights(this->lights);
	for(unsigned int i = 0; i < this->objects.size(); i++)
	{
		Object* object = this->objects.at(i);
		object->update();
	}
}

//Objects
void World::addObject(Object* object)
{
	this->objects.push_back(object);
}
void World::removeObject(Object* object)
{
	std::vector<Object*>::iterator it;
	it = std::find(this->objects.begin(),this->objects.end(),object);
	if(it != this->objects.end())
	{
		this->objects.erase(it);
	}
}

//RenderObjects
void World::addRenderObject(RenderObject* renderObject)
{
	this->renderObjects.push_back(renderObject);
	this->addObject(renderObject);
}
void World::removeRenderObject(RenderObject* renderObject)
{
	std::vector<RenderObject*>::iterator it;
	it = std::find(this->renderObjects.begin(),this->renderObjects.end(),renderObject);
	if(it != this->renderObjects.end())
	{
		this->renderObjects.erase(it);
	}
	this->removeObject(renderObject);
}
RenderObject* World::getRenderObject(int index)
{
	return this->renderObjects.at(index);
}

//Lights
void World::addLight(Light* light)
{
	this->lights.push_back(light);
	this->addObject(light);
}
void World::removeLight(Light* light)
{
	std::vector<Light*>::iterator it;
	it = std::find(this->lights.begin(),this->lights.end(),light);
	if(it != this->lights.end())
	{
		this->lights.erase(it);
	}
	this->removeObject(light);
}