#pragma once

#include <string>

#include "Object.h"
#include "../OpenGL/GLMesh.h"
#include "../Material.h"
#include "../OpenGL/GLState.h"
#include "../MaterialDatabase.h"
#include "../MeshDatabase.h"
#include "../Singleton.h"

class RenderObject: public Object
{
public:
	RenderObject();
	RenderObject(std::string name);
	RenderObject(std::string name, std::string mesh, std::string material, std::string program);
	virtual ~RenderObject();

	virtual void update();
	virtual void render();

	void setMesh(std::string name);
	std::string getMesh();

	void setMaterial(std::string name);
	std::string getMaterial();

	void setProgram(std::string type);
	std::string getProgram();

protected:
	static std::string className;

	GLMesh* mesh;
	Material* material;
};