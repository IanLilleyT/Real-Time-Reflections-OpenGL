#include "Mesh.h"

Mesh::Mesh(){}
Mesh::~Mesh(){}

//VBOPair
VBOPair Mesh::getVBOPair()
{
	return this->vboPair;
}
void Mesh::createVBOPair()
{
	std::vector<GLfloat> vertexVBOData = std::vector<GLfloat>();
	std::vector<GLfloat> normalVBOData = std::vector<GLfloat>();
	std::vector<GLfloat> vboData = std::vector<GLfloat>();
	std::vector<GLushort> iboData = std::vector<GLushort>();

	for(unsigned int i = 0; i < faces.size(); i++)
	{
		Face* face = faces.at(i);
		std::vector<glm::vec3> vertices = face->getVertices();
		std::vector<glm::vec3> normals = face->getNormals();
		for(unsigned int i = 0; i < vertices.size(); i++)
		{
			glm::vec3 vertex = vertices.at(i);
			glm::vec3 normal = normals.at(i);
			vertexVBOData.push_back((GLfloat)vertex.x);
			vertexVBOData.push_back((GLfloat)vertex.y);
			vertexVBOData.push_back((GLfloat)vertex.z);
			normalVBOData.push_back((GLfloat)normal.x);
			normalVBOData.push_back((GLfloat)normal.y);
			normalVBOData.push_back((GLfloat)normal.z);
			iboData.push_back((GLushort)iboData.size());
		}
	}

	//Concat vertex and normal vbo data
	vboData.insert(vboData.end(), vertexVBOData.begin(), vertexVBOData.end());
	vboData.insert(vboData.end(), normalVBOData.begin(), normalVBOData.end());
	this->vboPair = VBOPair(vboData,iboData);
}

//Faces
std::vector<Face*> Mesh::getFaces()
{
	return this->faces;
}
void Mesh::addFace(Face* face)
{
	this->faces.push_back(face);
}

//Obj File Loader
Mesh* Mesh::openOBJ(std::string fileName)
{
	std::string line;
	std::ifstream objFile;

	objFile.open(fileName.c_str(), std::ifstream::in);
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
					float x = (float)atof(results.at(1).c_str());
					float y = (float)atof(results.at(2).c_str());
					float z = (float)atof(results.at(3).c_str());
					glm::vec3 vertexData = glm::vec3(x,y,z);
					intermediateVertices.push_back(vertexData);
				}
				//Normals
				else if(results.at(0) == "vn")
				{
					float x = (float)atof(results.at(1).c_str());
					float y = (float)atof(results.at(2).c_str());
					float z = (float)atof(results.at(3).c_str());
					glm::vec3 normalData = glm::vec3(x,y,z);
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
		mesh->createVBOPair();
		return mesh;
	}
	else
	{
		std::cout << "Unable to open obj file." << std::endl;
		return 0;
	}
}