#include "GLMesh.h"

GLMeshData::GLMeshData(){}
GLMeshData::~GLMeshData(){}

bool GLMeshData::initialize(std::vector<GLfloat> vboData, std::vector<GLushort> iboData, 
	GLuint numElements, GLenum drawType)
{
	this->vertexBufferData = vboData;
	this->indexBufferData = iboData;
	this->drawType = drawType;
	this->numElements = numElements;
	return true;
}
bool GLMeshData::initialize(std::string filename)
{
	if(Utils::stringContains(filename,".obj"))
		return this->initializeOBJ(filename);
	return false;
}
bool GLMeshData::initializeOBJ(std::string filename)
{
	std::string line;
	std::ifstream file;
	std::vector<std::string> results;

	file.open(filename.c_str(), std::ifstream::in);
	if(file.is_open())
	{
		std::vector<glm::vec3> intermediateVertices = std::vector<glm::vec3>();
		std::vector<glm::vec3> intermediateNormals = std::vector<glm::vec3>();
		std::vector<GLfloat> vertexVBOData = std::vector<GLfloat>();
		std::vector<GLfloat> normalVBOData = std::vector<GLfloat>();
		std::vector<GLfloat> vboData = std::vector<GLfloat>();
		std::vector<GLushort> iboData = std::vector<GLushort>();
		int numElements = 0;
		glm::vec3 min = glm::vec3(FLT_MAX);
		glm::vec3 max = glm::vec3(FLT_MIN);

		while(file.good())
		{
			getline(file,line);
			results = Utils::splitByCharacter(line,' ');
			if(results.size() > 0)
			{
				std::string name = results.at(0);
				if(name == "v") //Vertex
				{
					glm::vec3 vertex = Utils::parseIntoVec3(line);
					intermediateVertices.push_back(vertex);
					if(vertex.x < min.x) min.x = vertex.x;
					if(vertex.y < min.y) min.y = vertex.y;
					if(vertex.z < min.z) min.z = vertex.z;
					if(vertex.x > max.x) max.x = vertex.x;
					if(vertex.y > max.y) max.y = vertex.y;
					if(vertex.z > max.z) max.z = vertex.z;
				}
				else if(name == "vn") //Normals
					intermediateNormals.push_back(Utils::parseIntoVec3(line));
				else if(name == "f") //Face
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
		this->boundingBox = new BoundingBox(min,max);
		return this->initialize(vboData,iboData,(GLuint)numElements,GL_TRIANGLES);
	}
	else
	{
		std::cout << "Unable to open mesh file: " << filename << std::endl;
		return false;
	}
}