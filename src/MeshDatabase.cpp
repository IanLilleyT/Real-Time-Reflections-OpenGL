#include "MeshDatabase.h"

MeshDatabase::MeshDatabase(){}
MeshDatabase::~MeshDatabase(){}

Mesh* MeshDatabase::loadMesh(std::string meshFileName)
{
	Mesh* newMesh = 0;
	newMesh = this->findMesh(meshFileName);
	if(newMesh == 0) 
		newMesh = this->openMeshFile(meshFileName);
	if(newMesh != 0)
		this->meshMap[meshFileName] = newMesh;
	return newMesh;
}
Mesh* MeshDatabase::findMesh(std::string meshFileName)
{
	std::map<std::string, Mesh*>::iterator it;
	it = this->meshMap.find(meshFileName);
	if(it != this->meshMap.end())
		return it->second;
	else
		return 0;
}
Mesh* MeshDatabase::openMeshFile(std::string meshFileName)
{
	std::string meshPath = Singleton<GlobalValues>::Instance()->getMeshPath();
	std::string fullPath = meshPath + meshFileName;

	std::string line;
	std::ifstream objFile;

	objFile.open(fullPath.c_str(), std::ifstream::in);
	if(objFile.is_open())
	{
		std::vector<glm::vec3> intermediateVertices = std::vector<glm::vec3>();
		std::vector<glm::vec3> intermediateNormals = std::vector<glm::vec3>();
		Mesh* mesh = new Mesh();
		while(objFile.good())
		{
			getline(objFile,line);
			std::vector<std::string> results = Utils::splitByCharacter(line,' ');
			if(results.size() > 0)
			{
				//Vertex
				if((results.at(0)) == "v")
				{
					glm::vec3 vertexData = Utils::parseIntoVec3(line);
					intermediateVertices.push_back(vertexData);
				}
				//Normals
				else if(results.at(0) == "vn")
				{
					glm::vec3 normalData = Utils::parseIntoVec3(line);
					intermediateNormals.push_back(normalData);
				}
				//Face
				else if(results.at(0) == "f")
				{
					Face* face = new Face();
					mesh->addFace(face);
					for(unsigned int i = 1; i < results.size(); i++)
					{
						std::vector<std::string> elementVector = Utils::splitByCharacter(results.at(i),'/');
						int vertexIndex = atoi(elementVector.at(0).c_str())-1;
						int normalIndex = atoi(elementVector.at(2).c_str())-1;
						glm::vec3 intermediateVertex = intermediateVertices.at(vertexIndex);
						glm::vec3 intermediateNormal = intermediateNormals.at(normalIndex);
						face->addVertexAndNormal(intermediateVertex,intermediateNormal);
					}
				}
			}
		}
		objFile.close();
		mesh->createGLMesh();
		return mesh;
	}
	else
	{
		std::cout << "Unable to open mesh file." << std::endl;
		return 0;
	}
}