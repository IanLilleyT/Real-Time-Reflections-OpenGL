#pragma once

#include <string>

#include "Object.h"
#include "../OpenGL/GLMesh.h"
#include "../Material.h"
#include "../OpenGL/GLState.h"
#include "../MaterialDatabase.h"
#include "../Singleton.h"

class RenderObject: public Object
{
public:
	RenderObject();
	virtual ~RenderObject();

	virtual void update();
	virtual void render();

	void setMesh(GLMesh* mesh);
	GLMesh* getMesh();

	void setMaterial(std::string name);
	void setMaterial(Material* material);
	Material* getMaterial();

	void setProgramType(std::string type);
	std::string getProgramType();

private:
	GLMesh* mesh;
	Material* material;
};