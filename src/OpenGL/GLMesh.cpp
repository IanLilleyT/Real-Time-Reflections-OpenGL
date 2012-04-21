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
    glGenBuffers(1, &this->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
	glGenBuffers(1, &this->indexBuffer);

    //Generate and bind the vao
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    //Enable attributes
	size_t offset = 0;
	std::vector<GLAttribute*> attributes = this->program->getAttributes();
    for(unsigned int i = 0; i < attributes.size(); i++)
    {
        GLAttribute* attribute = attributes.at(i);
        glEnableVertexAttribArray(attribute->getAttributePos()); //enable attribute
        glVertexAttribPointer(attribute->getAttributePos(), attribute->getSize(), attribute->getType(), GL_FALSE, 0, (void*) offset);
		offset += this->meshData->numElements*attribute->getSize()*sizeof(GLfloat);
    }

	//Put data in ARRAY buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer); //bind vertex buffer
	glBufferData(GL_ARRAY_BUFFER, this->meshData->vertexBufferData.size()*sizeof(GLfloat),
		&(this->meshData->vertexBufferData.at(0)), GL_STATIC_DRAW); //put vertex array data in vertex buffer

	//Put data in INDEX buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBuffer); //bind index buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->meshData->indexBufferData.size()*sizeof(GLushort),
		&(this->meshData->indexBufferData.at(0)), GL_STATIC_DRAW); //put index array data in the index buffer

    //Unbind
    glBindVertexArray(0); //unbind vao
}
GLProgram* GLMesh::getProgram()
{
	return this->program;
}
void GLMesh::Render()
{
	//Bind program and vao
	glUseProgram(this->program->getProgram());
	glBindVertexArray(this->vao);

	this->program->fillUniforms();
	glDrawElements(this->meshData->drawType, this->meshData->indexBufferData.size(), GL_UNSIGNED_SHORT, 0);

	//Unbind program and vao
	glBindVertexArray(0);
	glUseProgram(0);
}

//GLMeshData
void GLMesh::setGLMeshData(std::vector<GLfloat> vboData, std::vector<GLushort> iboData, 
	GLuint numElements, GLenum drawType)
{
	GLMeshData* newMeshData = new GLMeshData();
	newMeshData->initialize(vboData, iboData, numElements, drawType);
	this->meshData = newMeshData;
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
