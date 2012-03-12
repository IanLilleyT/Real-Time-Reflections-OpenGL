#pragma once

#include <string> //For names
#include <map> //For storing meshes

#include "OpenGL/GLMesh.h" //For storing meshes
#include "GlobalPaths.h" //For getting mesh path
#include "Singleton.h" //For using global classes

class MeshDatabase
{
public:
	MeshDatabase();
	virtual ~MeshDatabase();

	GLMesh* loadMesh(std::string meshName);

	static std::string NONE;

private:
	GLMesh* findMesh(std::string meshName);
	GLMesh* openMeshFile(std::string meshName);
	std::map<std::string,GLMesh*> meshMap;
};