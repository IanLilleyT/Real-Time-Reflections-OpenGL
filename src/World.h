#pragma once

#include <algorithm>
#include <vector>

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

	void addRenderObject(RenderObject* object);
	void removeRenderObject(RenderObject* object);
	RenderObject* getRenderObject(int index);

	void addLight(Light* light);
	void removeLight(Light* light);

private:
	void addObject(Object* object);
	void removeObject(Object* object);

	std::vector<Object*> objects;
	std::vector<RenderObject*> renderObjects;
	std::vector<Light*> lights;
};