#pragma once

#include <algorithm>
#include <vector>

#include "Objects/Object.h"
#include "Objects/RenderObject.h"
#include "Objects/Light.h"

class World
{
public:
	World();
	~World();

	void update();

	void addRenderObject(RenderObject* object);
	void removeRenderObject(RenderObject* object);

	void addLight(Light* light);
	void removeLight(Light* light);

private:
	void addObject(Object* object);
	void removeObject(Object* object);

	std::vector<Object*> objects;
	std::vector<RenderObject*> renderObjects;
	std::vector<Light*> lights;
};