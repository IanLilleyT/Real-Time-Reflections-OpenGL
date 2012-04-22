#include "MeshDatabase.h"

std::string MeshDatabase::NONE = "NONE";

MeshDatabase::MeshDatabase(){}
MeshDatabase::~MeshDatabase(){}

GLMeshData* MeshDatabase::loadMesh(std::string meshName)
{
	GLMeshData* meshData = 0;
	if(meshName != MeshDatabase::NONE)
	{
		meshData = this->findMesh(meshName);
		if(meshData == 0) 
		{
			meshData = this->openMeshFile(meshName);
			if(meshData != 0)
				this->meshMap[meshData->name] = meshData;
		}
	}
	return meshData;
}
GLMeshData* MeshDatabase::findMesh(std::string meshName)
{
	std::map<std::string, GLMeshData*>::iterator it;
	it = this->meshMap.find(meshName);
	if(it != this->meshMap.end())
		return it->second;
	else
		return 0;
}
GLMeshData* MeshDatabase::openMeshFile(std::string meshName)
{
	std::string meshPath = Singleton<GlobalPaths>::Instance()->getMeshPath();
	std::string fullPath = meshPath + meshName + ".obj";

	GLMeshData* meshData = new GLMeshData();
	meshData->initialize(fullPath);
	meshData->name = meshName;

	return meshData;
}