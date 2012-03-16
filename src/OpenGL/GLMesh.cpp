#include "GLMesh.h"

GLMesh::GLMesh(){}
GLMesh::~GLMesh(){}

/*-----------------------------------------------------------------------------
//// GLMesh ///////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------------*/

//Program
void GLMesh::setProgram(std::string programName)
{
	//Make sure to call setGLMeshData first!
	this->program = Singleton<GLProgramDatabase>::Instance()->loadProgram(programName);
    
	//Generate and bind the array buffer
    glGenBuffers(1, &this->vertexBuffer); //generate vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer); //bind vertex buffer
	glGenBuffers(1, &this->indexBuffer); //generate index buffer

    //Vertex array object setup
	GLuint vao = this->program->getVAO()->getVertexArrayObject();
    glGenVertexArrays(1, &vao);
    this->program->getVAO()->setVertexArrayObject(vao);
    glBindVertexArray(this->program->getVAO()->getVertexArrayObject()); //bind vao

    //Enable attributes
	size_t offset = 0;
	std::vector<GLAttribute*> attributes = this->program->getVAO()->getAttributes();
    for(unsigned int i = 0; i < attributes.size(); i++)
    {
        GLAttribute* attribute = attributes.at(i);
        glEnableVertexAttribArray(attribute->getAttributePos()); //enable attribute
        glVertexAttribPointer(attribute->getAttributePos(), attribute->getSize(), attribute->getType(), GL_FALSE, 0, (void*) offset);
		offset += this->meshData->numElements*attribute->getSize()*sizeof(GLfloat);
    }

    //Unbind
    glBindVertexArray(0); //unbind vao
}
std::string GLMesh::getProgram()
{
	return this->program->getName();
}
void GLMesh::Render()
{
	//Put data in VERTEX buffer
	glUseProgram(this->program->getProgram()); //bind program
	glBindVertexArray(this->program->getVAO()->getVertexArrayObject()); //bind vao
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer); //bind vertex buffer
	glBufferData(GL_ARRAY_BUFFER, this->meshData->vertexBufferData.size()*sizeof(GLfloat),
		&(this->meshData->vertexBufferData.at(0)), GL_DYNAMIC_DRAW); //put vertex array data in vertex buffer

	//Fill uniforms
	this->program->fillUniforms();

	//Draw the mesh
	//Put data in INDEX buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBuffer); //bind index buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->meshData->indexBufferData.size()*sizeof(GLushort),
		&(this->meshData->indexBufferData.at(0)), GL_DYNAMIC_DRAW); //put index array data in the index buffer
	glDrawElements(this->meshData->drawType, this->meshData->indexBufferData.size(), GL_UNSIGNED_SHORT, 0); //Draw on the screen

	//Unbind
	glBindVertexArray(0); //unbind vao
	glUseProgram(0); //unbind program
}

//GLMeshData
void GLMesh::setGLMeshData(std::vector<GLfloat> vboData, std::vector<GLushort> iboData, 
	GLuint numElements, GLenum drawType)
{
	this->meshData = new GLMeshData(vboData, iboData, numElements, drawType);
}
void GLMesh::setGLMeshData(GLMeshData* meshData)
{
	this->meshData = meshData;
}
GLMeshData* GLMesh::getGLMeshData()
{
	return this->meshData;
}
std::vector<GLfloat>& GLMesh::getVBOData()
{
	return this->meshData->vertexBufferData;
}
std::vector<GLushort>& GLMesh::getIBOData()
{
	return this->meshData->indexBufferData;
}
int GLMesh::getNumElements()
{
	return this->meshData->numElements;
}

//Name
void GLMesh::setName(std::string name)
{
	this->name = name;
}
std::string GLMesh::getName()
{
	return this->name;
}

//Visible
void GLMesh::setVisible(bool visible)
{
	this->visible = visible;
}
bool GLMesh::isVisible()
{
	return this->visible;
}

/*-----------------------------------------------------------------------------
//// GLMeshData ///////////////////////////////////////////////////////////////
-----------------------------------------------------------------------------*/
GLMeshData::GLMeshData(std::vector<GLfloat> vboData, std::vector<GLushort> iboData, GLuint numElements, GLenum drawType)
{
	this->vertexBufferData = vboData;
	this->indexBufferData = iboData;
	this->drawType = drawType;
	this->numElements = numElements;
}
GLMeshData::~GLMeshData(){}

GLMeshData* GLMeshData::initialize(std::string filename)
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
		return new GLMeshData(vboData,iboData,(GLuint)numElements,GL_TRIANGLES);
	}
	else
	{
		std::cout << "Unable to open mesh file." << std::endl;
		return 0;
	}
}