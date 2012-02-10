#include "RenderObject.h"

RenderObject::RenderObject(){}
RenderObject::~RenderObject(){}

void RenderObject::update()
{
	this->render();
}
void RenderObject::render()
{
	Singleton<GLState>::Instance()->setModelToWorldMatrix(this->transformationMatrix);
	Singleton<GLState>::Instance()->setMaterial(this->material);
	this->mesh->Render();
}

//Mesh
void RenderObject::setMesh(GLMesh* mesh)
{
	this->mesh = mesh;
}
GLMesh* RenderObject::getMesh()
{
	return this->mesh;
}

//Material
void RenderObject::setMaterial(std::string material)
{
	Material* newMaterial = Singleton<MaterialDatabase>::Instance()->loadMaterial(material);
	this->setMaterial(newMaterial);
}
void RenderObject::setMaterial(Material* material)
{
	this->material = material;
}
Material* RenderObject::getMaterial()
{
	return this->material;
}

//Program Type
void RenderObject::setProgramType(std::string programType)
{
	this->mesh->setProgramType(programType);
}
std::string RenderObject::getProgramType()
{
	return this->mesh->getProgramType();
}