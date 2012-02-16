#include "GLMesh.h"

GLMesh::GLMesh()
{
	this->program = 0;
    this->vertexArrayObject = 0;
    this->bufferObject = new GLBufferObject();
}
GLMesh::~GLMesh()
{
    delete this->program;
	delete this->vertexArrayObject;
	delete this->bufferObject;
}

/*---------------------------------------------
  Setters
---------------------------------------------*/
void GLMesh::setProgram(std::string programName)
{
	GLProgram* program = Singleton<GLProgramDatabase>::Instance()->loadProgram(programName);
	this->program = program;
	this->program->createVAO();
	this->vertexArrayObject = this->program->getVAO(); //Only fills the vao attributes, not the GLuint
    this->Generate();
}
std::string GLMesh::getProgram()
{
	return this->program->getName();
}
void GLMesh::setVBOData(std::vector<GLfloat> vboData, std::vector<GLushort> iboData, GLuint numElements, GLenum drawType)
{
	this->bufferObject->setVertexBufferData(vboData);
	this->bufferObject->setIndexBufferData(iboData);
	this->bufferObject->setDrawType(drawType);
	this->bufferObject->setNumElements(numElements);
}
std::vector<GLfloat>& GLMesh::getVBOData()
{
	return this->bufferObject->getVertexBufferData();
}
std::vector<GLushort>& GLMesh::getIBOData()
{
	return this->bufferObject->getIndexBufferData();
}
int GLMesh::getNumElements()
{
	return this->bufferObject->getNumElements();
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

/*---------------------------------------------
  Other
---------------------------------------------*/
void GLMesh::Generate()
{
	//Make sure to fill the vbo before setting the program (which calls this method)
    //Generate and bind the array buffer
    GLuint vertexBuffer = this->bufferObject->getVertexBuffer();
    glGenBuffers(1, &vertexBuffer); //generate vertex buffer
    this->bufferObject->setVertexBuffer(vertexBuffer); //set the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, this->bufferObject->getVertexBuffer()); //bind vertex buffer

    //Vertex array object setup
    GLuint vao = this->vertexArrayObject->getVertexArrayObject();
    glGenVertexArrays(1, &vao);
    this->vertexArrayObject->setVertexArrayObject(vao);
    glBindVertexArray(this->vertexArrayObject->getVertexArrayObject()); //bind vao

    //Enable attributes
	size_t offset = 0;
	std::vector<GLAttribute*> attributes = this->vertexArrayObject->getAttributes();
	GLuint numElements = this->bufferObject->getNumElements();
    for(unsigned int i = 0; i < attributes.size(); i++)
    {
        GLAttribute* attribute = attributes.at(i);
        glEnableVertexAttribArray(attribute->getAttributePos()); //enable attribute
        glVertexAttribPointer(attribute->getAttributePos(), attribute->getSize(), attribute->getType(), GL_FALSE, 0, (void*) offset);
		offset += numElements*attribute->getSize()*sizeof(GLfloat);
    }

    //Set up element buffer
    GLuint indexBuffer = this->bufferObject->getIndexBuffer();
    glGenBuffers(1, &indexBuffer); //generate index buffer
    this->bufferObject->setIndexBuffer(indexBuffer);

    //Unbind
    glBindVertexArray(0); //unbind vao
}
void GLMesh::Render()
{
	//Put data in VERTEX buffer
	glUseProgram(this->program->getProgram()); //bind program
	glBindVertexArray(this->vertexArrayObject->getVertexArrayObject()); //bind vao
	std::vector<GLfloat>& bufferData = this->bufferObject->getVertexBufferData();
	glBindBuffer(GL_ARRAY_BUFFER, this->bufferObject->getVertexBuffer()); //bind vertex buffer
	glBufferData(GL_ARRAY_BUFFER, bufferData.size()*sizeof(GLfloat),
					&(bufferData.at(0)), GL_DYNAMIC_DRAW); //put vertex array data in vertex buffer

	//Fill uniforms
	this->program->fillUniforms();

	//Draw the mesh
	//Put data in INDEX buffer
	std::vector<GLushort>& indexData = this->bufferObject->getIndexBufferData();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->bufferObject->getIndexBuffer()); //bind index buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size()*sizeof(GLushort),
					&(indexData.at(0)), GL_DYNAMIC_DRAW); //put index array data in the index buffer
	glDrawElements(this->bufferObject->getDrawType(), indexData.size(), GL_UNSIGNED_SHORT, 0); //Draw on the screen

	//Unbind
	glBindVertexArray(0); //unbind vao
	glUseProgram(0); //unbind program
}
void GLMesh::setVisible(bool visible)
{
	this->visible = visible;
}
bool GLMesh::isVisible()
{
	return this->visible;
}
