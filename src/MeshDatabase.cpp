#include "MeshDatabase.h"

std::string MeshDatabase::NONE = "NONE";

MeshDatabase::MeshDatabase(){}
MeshDatabase::~MeshDatabase(){}

GLMesh* MeshDatabase::loadMesh(std::string meshName)
{
	GLMesh* mesh = 0;
	GLMeshData* meshData = this->findMesh(meshName);
	if(meshData == 0) 
	{
		meshData = this->openMeshFile(meshName);
		if(meshData != 0)
			this->meshMap[meshData->name] = meshData;
	}
	if(meshData != 0)
	{
		mesh = new GLMesh();
		mesh->setGLMeshData(meshData);
	}
	return mesh;
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

	GLMeshData* meshData = GLMeshData::initialize(fullPath);
	meshData->name = meshName;
	return meshData;
}