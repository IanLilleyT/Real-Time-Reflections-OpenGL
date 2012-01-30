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
void GLMesh::setProgramType(std::string programName)
{
	GLProgram* program = Singleton<GLProgramDatabase>::Instance()->loadProgram(programName);
	this->program = program;
	this->vertexArrayObject = this->program->getVAO(); //Only fills the vao attributes, not the GLuint
    this->Generate();
}

/*---------------------------------------------
  Getters
---------------------------------------------*/
GLProgram* GLMesh::getProgram()
{
    return this->program;
}
GLBufferObject* GLMesh::getBufferObject()
{
    return this->bufferObject;
}
GLVertexArrayObject* GLMesh::getVertexArrayObject()
{
    return this->vertexArrayObject;
}

/*---------------------------------------------
  Other
---------------------------------------------*/
void GLMesh::Generate()
{
	//Make sure to fill the vbo before setting the program (which calls this method)
    //Generate and bind the array buffer
    GLuint vertexBuffer = this->getBufferObject()->getVertexBuffer();
    glGenBuffers(1, &vertexBuffer); //generate vertex buffer
    this->getBufferObject()->setVertexBuffer(vertexBuffer); //set the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, this->getBufferObject()->getVertexBuffer()); //bind vertex buffer

    //Vertex array object setup
    GLuint vao = this->getVertexArrayObject()->getVertexArrayObject();
    glGenVertexArrays(1, &vao);
    this->getVertexArrayObject()->setVertexArrayObject(vao);
    glBindVertexArray(this->getVertexArrayObject()->getVertexArrayObject()); //bind vao

    //Enable attributes
	size_t offset = 0;
	GLuint numElements = this->getBufferObject()->getNumElements();
	std::vector<GLAttribute*> attributes = this->getVertexArrayObject()->getAttributes();
    for(unsigned int j = 0; j < attributes.size(); j++)
    {
        GLAttribute* attribute = attributes.at(j);
        glEnableVertexAttribArray(attribute->getAttributePos()); //enable attribute
        glVertexAttribPointer(attribute->getAttributePos(), attribute->getSize(), attribute->getType(), GL_FALSE, 0, (void*) offset);
		offset += numElements*attribute->getSize()*sizeof(GLfloat);
    }

    //Set up element buffer
    GLuint indexBuffer = this->getBufferObject()->getIndexBuffer();
    glGenBuffers(1, &indexBuffer); //generate index buffer
    this->getBufferObject()->setIndexBuffer(indexBuffer);

    //Unbind
    glBindVertexArray(0); //unbind vao
}
void GLMesh::Render()
{
    //Put data in VERTEX buffer
    glUseProgram(this->getProgram()->getProgram()); //bind program
    glBindVertexArray(this->getVertexArrayObject()->getVertexArrayObject()); //bind vao
	std::vector<GLfloat> bufferData = this->getBufferObject()->getVertexBufferData();
    glBindBuffer(GL_ARRAY_BUFFER, this->getBufferObject()->getVertexBuffer()); //bind vertex buffer
    glBufferData(GL_ARRAY_BUFFER, bufferData.size()*sizeof(GLfloat),
                 &(bufferData.at(0)), GL_DYNAMIC_DRAW); //put vertex array data in vertex buffer

    //Fill uniforms
	this->getProgram()->fillUniforms();

    //Draw the mesh
    //Put data in INDEX buffer
	std::vector<GLushort> indexData = this->getBufferObject()->getIndexBufferData();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->getBufferObject()->getIndexBuffer()); //bind index buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size()*sizeof(GLushort),
                 &(indexData.at(0)), GL_DYNAMIC_DRAW); //put index array data in the index buffer
    glDrawElements(this->getBufferObject()->getDrawType(), indexData.size(), GL_UNSIGNED_SHORT, 0); //Draw on the screen

    //Unbind
    glBindVertexArray(0); //unbind vao
    glUseProgram(0); //unbind program
}
