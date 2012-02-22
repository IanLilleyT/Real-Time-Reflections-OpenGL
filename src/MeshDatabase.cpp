#include "MeshDatabase.h"

std::string MeshDatabase::NO_NAME = "NO_NAME";

MeshDatabase::MeshDatabase(){}
MeshDatabase::~MeshDatabase(){}

GLMesh* MeshDatabase::loadMesh(std::string meshName)
{
	GLMesh* newMesh = 0;
	newMesh = this->findMesh(meshName);
	if(newMesh == 0) 
	{
		newMesh = this->openMeshFile(meshName);
		if(newMesh != 0)
			this->meshMap[meshName] = newMesh;
	}
	return newMesh;
}
GLMesh* MeshDatabase::findMesh(std::string meshName)
{
	std::map<std::string, GLMesh*>::iterator it;
	it = this->meshMap.find(meshName);
	if(it != this->meshMap.end())
		return it->second;
	else
		return 0;
}
GLMesh* MeshDatabase::openMeshFile(std::string meshName)
{
	std::string meshPath = Singleton<GlobalPaths>::Instance()->getMeshPath();
	std::string fullPath = meshPath + meshName + ".obj";

	GLMesh* mesh = new GLMesh();
	mesh->initialize(fullPath);
	mesh->setName(meshName); //TODO: get name from somewhere else

	return mesh;
}