#include "GLMesh.h"

GLMesh::GLMesh(){}
GLMesh::~GLMesh(){}

//Initialize
void GLMesh::initialize(std::vector<GLfloat> vboData, std::vector<GLushort> iboData, 
	GLuint numElements, GLenum drawType)
{
	GLMeshData* newMeshData = new GLMeshData();
	newMeshData->initialize(vboData, iboData, numElements, drawType);
	this->initialize(newMeshData);
}
void GLMesh::initialize(GLMeshData* meshData)
{
	this->meshData = meshData;
	this->Generate();
}
GLMeshData* GLMesh::getGLMeshData()
{
	return this->meshData;
}

//Rendering
void GLMesh::Generate()
{
	//Generate and bind the array buffer
    glGenBuffers(1, &this->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
	glGenBuffers(1, &this->indexBuffer);

    //Generate and bind the vao
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    //Enable attributes
	size_t offset = 0;
    for(int i = 0; i < this->meshData->numAttributes; i++)
    {
		glEnableVertexAttribArray(i); //enable attribute
		glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, (void*) offset);
		offset += this->meshData->numElements*3*sizeof(GLfloat);
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
void GLMesh::Render()
{
	//Decide if we should use a global program or not
	GLProgram* programToUse = this->program;
	std::string globalProgramString = Singleton<GLState>::Instance()->globalProgramName;
	GLProgram* globalProgram = Singleton<GLProgramDatabase>::Instance()->loadProgram(globalProgramString);
	if(globalProgram != 0)
		programToUse = globalProgram;

	//Bind program and vao
	glUseProgram(programToUse->getProgram());
	glBindVertexArray(this->vao);

	programToUse->fillUniforms();
	glDrawElements(this->meshData->drawType, this->meshData->indexBufferData.size(), GL_UNSIGNED_SHORT, 0);

	//Unbind program and vao
	glBindVertexArray(0);
	glUseProgram(0);
}

//Program
void GLMesh::setProgram(std::string programName)
{
	this->program = Singleton<GLProgramDatabase>::Instance()->loadProgram(programName);
}
GLProgram* GLMesh::getProgram()
{
	return this->program;
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
