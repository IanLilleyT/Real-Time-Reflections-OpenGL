#include "GLProgram_Material.h"

GLProgram_Material::GLProgram_Material() : GLProgram()
{
	this->createVAO();
}
GLProgram_Material::~GLProgram_Material(){}

void GLProgram_Material::createVAO()
{
	//Only fills the vao attributes, not the GLuint
	this->vao = new GLVertexArrayObject();
	GLAttribute* positionAttribute = this->getAttribute("position", 3, GL_FLOAT);
	GLAttribute* normalAttribute = this->getAttribute("normal", 3, GL_FLOAT);
	std::vector<GLAttribute*> attributes = std::vector<GLAttribute*>();
	attributes.push_back(positionAttribute);
	attributes.push_back(normalAttribute);
	this->vao->setAttributes(attributes);
}
void GLProgram_Material::bindUniformBlocks()
{
	this->uniformBlocks.push_back(GLUniformBlockHelper::TYPE_PROJECTION);
	this->uniformBlocks.push_back(GLUniformBlockHelper::TYPE_LIGHTS);
	GLProgram::bindUniformBlocks();
}
void GLProgram_Material::fillUniforms()
{
	GLState* glState = Singleton<GLState>::Instance();
	glm::mat4 modelToWorldMatrix = glState->getModelToWorldMatrix();
	glm::mat4 worldToCameraMatrix = glState->getWorldToCameraMatrix();
	glm::mat4 modelToCameraMatrix = worldToCameraMatrix * modelToWorldMatrix;
	Material* material = glState->getMaterial();

	//Model to camera
	GLuint modelToCameraMatrixUniform = glGetUniformLocation(this->program, "modelToCameraMatrix");
	glUniformMatrix4fv(modelToCameraMatrixUniform, 1, GL_FALSE, glm::value_ptr(modelToCameraMatrix));

	//Normal model to camera
	glm::mat3 normalModelToCameraMatrix = glm::mat3(modelToCameraMatrix);
	normalModelToCameraMatrix = glm::transpose(glm::inverse(normalModelToCameraMatrix)); //Normal transformation
	GLuint normalModelToCameraMatrixUniform = glGetUniformLocation(this->program, "normalModelToCameraMatrix");
	glUniformMatrix3fv(normalModelToCameraMatrixUniform, 1, GL_FALSE, glm::value_ptr(normalModelToCameraMatrix));

	//In Diffuse Color
	glm::vec4 diffuseColor = material->getDiffuseColor();
	GLuint diffuseColorUniform = glGetUniformLocation(this->program, "diffuseColor");
	glUniform4fv(diffuseColorUniform, 1, glm::value_ptr(diffuseColor));

	//Specular Color
	glm::vec4 specularColor = material->getSpecularColor();
	GLuint specularColorUniform = glGetUniformLocation(this->program, "specularColor");
	glUniform4fv(specularColorUniform, 1, glm::value_ptr(specularColor));

	//Specular Shininess
	float shininessFactor = material->getSpecularShininess();
	GLuint shininessFactorUniform = glGetUniformLocation(this->program, "specularShininess");
	glUniform1f(shininessFactorUniform, shininessFactor);

	//Alpha
	float alpha = material->getAlpha();
	GLuint alphaUniform = glGetUniformLocation(this->program, "alpha");
	glUniform1f(alphaUniform,alpha);

	//Num Lights
	int numLights = glState->getLights().size();
	GLuint numLightsUniform = glGetUniformLocation(this->program, "numLights");
	glUniform1i(numLightsUniform, numLights);
}