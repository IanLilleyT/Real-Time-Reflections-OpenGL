#include "Mesh.h"

Mesh::Mesh(){}
Mesh::~Mesh(){}

void Mesh::createGLMesh()
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

	this->glMesh = new GLMesh();
	this->glMesh->getBufferObject()->setVertexBufferData(vboData);
	this->glMesh->getBufferObject()->setIndexBufferData(iboData);
	this->glMesh->getBufferObject()->setDrawType(GL_TRIANGLES);
}

//GLMesh
void Mesh::setGLMesh(GLMesh* glMesh)
{
	this->glMesh = glMesh;
}
GLMesh* Mesh::getGLMesh()
{
	return this->glMesh;
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

//Program types
std::string Mesh::getProgramType()
{
	return this->glMesh->getProgram()->getName();
}
void Mesh::setProgramType(std::string programType)
{
	this->glMesh->setProgramType(programType);
}