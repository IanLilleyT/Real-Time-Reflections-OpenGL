#include "MeshDatabase.h"

MeshDatabase::MeshDatabase(){}
MeshDatabase::~MeshDatabase(){}

GLMesh* MeshDatabase::loadMesh(std::string meshFileName)
{
	GLMesh* newMesh = 0;

	std::string extension = ".obj";
	if(meshFileName.find(extension) == std::string::npos)
		meshFileName += extension;

	newMesh = this->findMesh(meshFileName);
	if(newMesh == 0) 
	{
		newMesh = this->openMeshFile(meshFileName);
		if(newMesh != 0)
			this->meshMap[meshFileName] = newMesh;
	}
	return newMesh;
}
GLMesh* MeshDatabase::findMesh(std::string meshFileName)
{
	std::map<std::string, GLMesh*>::iterator it;
	it = this->meshMap.find(meshFileName);
	if(it != this->meshMap.end())
		return it->second;
	else
		return 0;
}
void MeshDatabase::copyMesh(std::string meshFileName, std::string newName)
{
	if(this->loadMesh(newName) == 0)
	{
		GLMesh* mesh = this->loadMesh(meshFileName);
		this->meshMap[newName] = new GLMesh(*mesh);
	}
}
GLMesh* MeshDatabase::openMeshFile(std::string meshFileName)
{
	std::string meshPath = Singleton<GlobalPaths>::Instance()->getMeshPath();
	std::string fullPath = meshPath + meshFileName;

	std::string line;
	std::ifstream file;
	std::vector<std::string> results;

	file.open(fullPath.c_str(), std::ifstream::in);
	if(file.is_open())
	{
		std::vector<glm::vec3> intermediateVertices = std::vector<glm::vec3>();
		std::vector<glm::vec3> intermediateNormals = std::vector<glm::vec3>();
		std::vector<GLfloat> vertexVBOData = std::vector<GLfloat>();
		std::vector<GLfloat> normalVBOData = std::vector<GLfloat>();
		std::vector<GLfloat> vboData = std::vector<GLfloat>();
		std::vector<GLushort> iboData = std::vector<GLushort>();
		int numElements = 0;

		while(file.good())
		{
			getline(file,line);
			results = Utils::splitByCharacter(line,' ');
			if(results.size() > 0)
			{
				//Vertex
				std::string name = results.at(0);
				if(name == "v")
				{
					glm::vec3 vertexData = Utils::parseIntoVec3(line);
					intermediateVertices.push_back(vertexData);
				}
				//Normals
				else if(name == "vn")
				{
					glm::vec3 normalData = Utils::parseIntoVec3(line);
					intermediateNormals.push_back(normalData);
				}
				//Face
				else if(name == "f")
				{
					for(unsigned int i = 1; i < results.size(); i++)
					{
						std::vector<std::string> elementVector = Utils::splitByCharacter(results.at(i),'/');
						int vertexIndex = atoi(elementVector.at(0).c_str())-1;
						int normalIndex = atoi(elementVector.at(2).c_str())-1;
						glm::vec3 vertex = intermediateVertices.at(vertexIndex);
						glm::vec3 normal = intermediateNormals.at(normalIndex);
						vertexVBOData.push_back((GLfloat)vertex.x);
						vertexVBOData.push_back((GLfloat)vertex.y);
						vertexVBOData.push_back((GLfloat)vertex.z);
						normalVBOData.push_back((GLfloat)normal.x);
						normalVBOData.push_back((GLfloat)normal.y);
						normalVBOData.push_back((GLfloat)normal.z);
						iboData.push_back((GLushort)iboData.size());
						numElements++;
					}
				}
			}
		}
		file.close();

		vboData.insert(vboData.end(), vertexVBOData.begin(), vertexVBOData.end());
		vboData.insert(vboData.end(), normalVBOData.begin(), normalVBOData.end());
		GLMesh* mesh = new GLMesh();
		mesh->setVBOData(vboData,iboData,(GLuint)numElements,GL_TRIANGLES);
		mesh->setName(meshFileName);
		mesh->setVisible(true);
		return mesh;
	}
	else
	{
		std::cout << "Unable to open mesh file." << std::endl;
		return 0;
	}
}

bool MeshDatabase::isFieldValid(std::ifstream& file, std::string name, std::vector<std::string>& results)
{
	std::string line;
	getline(file, line);
	results = Utils::splitByCharacter(line, ' ');
	return results.at(0) == name;
}