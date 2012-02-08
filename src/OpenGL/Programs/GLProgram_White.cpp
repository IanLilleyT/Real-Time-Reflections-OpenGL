#include "GLProgram_White.h"

GLProgram_White::GLProgram_White()
{
	this->createVAO();
}
GLProgram_White::~GLProgram_White(){}
void GLProgram_White::createVAO()
{
    this->vao = new GLVertexArrayObject();
    GLAttribute* positionAttribute = this->getAttribute("position", 3, GL_FLOAT);
	std::vector<GLAttribute*> attributes = std::vector<GLAttribute*>();
	attributes.push_back(positionAttribute);
	this->vao->setAttributes(attributes);
}
void GLProgram_White::bindUniformBlocks()
{
	GLProgram::bindUniformBlocks();
}
void GLProgram_White::fillUniforms()
{
    //Model to clip
	GLState* glState = Singleton<GLState>::Instance();
	glm::mat4 modelToWorldMatrix = glState->getModelToWorldMatrix();
	glm::mat4 worldToCameraMatrix = glState->getWorldToCameraMatrix();
	glm::mat4 cameraToClipMatrix = glState->getCameraToClipMatrix();
	glm::mat4 modelToClipMatrix = cameraToClipMatrix * worldToCameraMatrix * modelToWorldMatrix;

    GLuint modelToClipMatrixUniform = glGetUniformLocation(this->program, "modelToClipMatrix");
    glUniformMatrix4fv(modelToClipMatrixUniform, 1, GL_FALSE, glm::value_ptr(modelToClipMatrix));
}
